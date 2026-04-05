import("stdfaust.lib");

// --- UI Controls ---


thresh  =hslider("[0] Threshold [unit:dB]", -20, -60, 0, 0.1);
ratio   =hslider("[1] Ratio", 4, 1, 20, 0.1);
knee    =  4;//t_grp(hslider("[2] Knee [unit:dB]", 3, 0, 20, 0.1));
// Convert ms to seconds for the pole calculation
att_s   =hslider("[3] Attack [unit:ms]", 10, 0.1, 500, 0.1)* 0.001;
rel_s   = hslider("[4] Release [unit:ms]", 100, 5, 2000, 1) * 0.001;

// --- Helper: Asymmetrical Smoother ---
// This smooths the Gain Reduction signal with different rates for up/down
// In Gain Reduction:
// Moving AWAY from 0dB (falling) = Attack
// Moving TOWARD 0dB (rising) = Release
ar_filter(a, r, x) = loop ~ _
with {
    // Calculate filter coefficients (poles) from time constants
    a_c = exp(-1.0 / (ma.SR * a));
    r_c = exp(-1.0 / (ma.SR * r));

    loop(y) = ba.if(x < y,
                x * (1 - a_c) + y * a_c, // Falling (Attack)
                x * (1 - r_c) + y * r_c  // Rising (Release)
              );
};

// --- Gain Reduction Meter ---
gr_meter(gr) = gr : hbargraph("Gain Reduction [unit:dB]", -60, 0);

// --- The Improved Algorithm ---
calc_gain(sc0, sc1) = gr_linear
with {
    // 1. Peak Detection
    sidechain_level = max(abs(sc0), abs(sc1));
    level_db = ba.linear2db(sidechain_level + ma.EPSILON);

    // 2. Gain Computer (Static Curve)
    // Determines how much the signal SHOULD be reduced
    gain_computer(input_db) = ba.if(input_db < (thresh - knee/2),
        0,
        ba.if(input_db > (thresh + knee/2),
            (1/ratio - 1) * (input_db - thresh),
            (1/ratio - 1) * (input_db - thresh + knee/2)^2 / (2 * knee)
        )
    );

    target_gr_db = gain_computer(level_db);

    // 3. Smooth the Gain Reduction signal
    gr_smoothed_db = ar_filter(att_s, rel_s, target_gr_db);

    // 4. Metering and Linear Conversion
    gr_linear = gr_smoothed_db : gr_meter : ba.db2linear;
};

// --- Routing ---
// Inputs 1-2: Main Audio | Inputs 3-4: Sidechain (The "Ducker" trigger)
sidechain_comp(in0, in1, sc0, sc1) = in0*g, in1*g
with {
    g = calc_gain(sc0, sc1);
};

// --- Debugging and Process ---
// Single input (mono) split to all 4 channels to test self-compression
debug_process = _ <: _, _, _, _ : sidechain_comp;

// Standard 4-channel operation
main_process = sidechain_comp;

// Toggle this to debug_process to test with a mono source!
process = main_process;
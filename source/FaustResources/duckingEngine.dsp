import("stdfaust.lib");

// --- UI Controls ---
thresh  = hslider("[0] Threshold [unit:dB]", -20, -60, 0, 0.1);
ratio   = hslider("[1] Ratio", 4, 1, 20, 0.1);
attack  = hslider("[2] Attack [unit:ms]", 10, 1, 500, 1) * 0.001;
release = hslider("[3] Release [unit:ms]", 100, 5, 2000, 1) * 0.001;

// --- Gain Reduction Meter ---
gr_meter(gr) = gr : hbargraph("[4] Gain Reduction [unit:dB]", -60, 0);

// --- The Ducking Logic ---
calc_gain(s1, s2) = gain_lin
with {
    sidechain_sum = max(abs(s1), abs(s2));
    env = sidechain_sum : en.ar(attack, release);
    env_db = ba.linear2db(env + ma.EPSILON);
    gr_db = min(0, (thresh - env_db) * (1 - 1/ratio));
    gain_lin = gr_db : gr_meter : ba.db2linear;
};

// --- Sidechain Compressor Function ---
sidechain_comp(in0, in1, sc0, sc1) = in0*g, in1*g
with {
    g = calc_gain(sc0, sc1);
};

// --- Main Process (4 Inputs -> 2 Outputs) ---
process = sidechain_comp;
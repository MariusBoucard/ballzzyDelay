import("stdfaust.lib");

// ===== Global Controls =====
gate      = button("gate");
globalMix = hslider("Global Mix", 0.5, 0, 1, 0.01);

// ===== Constants =====
MAXDELAYSAMPLES = int(ma.SR * 4);

// ===== The Delay Head Module =====
// 'i' is the index (0, 1, 2, 3) used to create unique labels
delayHead(i) = toogleOn *fbDelay(fb, dSamples) : sp.panner(pan) : *(level), *(level)
with {
    // Unique sliders for each head
    dTime    = hslider("Head %i/Time [unit:s]", 0.2 * (i+1), 0, 4, 0.001);
    pan      = hslider("Head %i/Pan", 0.5, 0, 1, 0.01);
    fb       = hslider("Head %i/Feedback", 0.3, 0, 0.95, 0.01);
    level    = hslider("Head %i/Level", 0, 0, 1, 0.01);
    toogleOn = checkbox("Head %i/on");
    
    dSamples = int(dTime * ma.SR);
    fbDelay(f, d) = _ : (+ : de.sdelay(MAXDELAYSAMPLES, 1024, d)) ~ *(f);
};

// ===== Djembe =====
djembe = pm.djembe(110, 0.3, 0.5, 0.8, gate);

// ===== Main Process =====
process = _ <: (dry, wet) :> _,_
with {
    dry = * (1 - globalMix) <: _,_;
    
    // Split the mono djembe into 4 delay heads and sum their stereo outputs
    // route: (1 in -> 4 heads -> 8 signals) :> (2 signals)
    wet = (par(i, 4, delayHead(i)) :> _,_) : *(globalMix), *(globalMix);
};
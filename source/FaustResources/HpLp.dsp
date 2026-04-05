import("stdfaust.lib");

// ===== Filter Controls =====
lpFreq   = hslider("Filters/[0]LPF [unit:Hz]", 20000, 20, 20000, 1) : si.smoo;
hpFreq   = hslider("Filters/[2]HPF [unit:Hz]", 20, 20, 20000, 1) : si.smoo;
bypassLp   = checkbox("Filters/[4]BypassLp");
bypassHp   = checkbox("Filters/[4]BypassHp");
// ===== Filter Chain =====
filterChain = ba.bypass1(bypassHp, fi.highpass(4, hpFreq)) : ba.bypass1(bypassLp,fi.lowpass(4, lpFreq));

// ===== Main Process =====
process = filterChain, filterChain;
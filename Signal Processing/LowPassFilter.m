function y = LowPassFilter(x)
    Fs = 1000; %sampling Frequency
    Fpass = 80; %Passband Frequency
    Fstop = 100; %Stopband Frequency
    Apass = 1;  % Passband Ripple (dB)
    Astop = 20;          % Stopband Attenuation (dB)
    match = 'stopband';  % Band to match exactly
    % Construct an FDESIGN object and call its BUTTER method.
    h  = fdesign.lowpass(Fpass, Fstop, Apass, Astop, Fs);
    Hd = butter(h, 'MatchExactly', match); 
    y = filter(Hd,x);
end
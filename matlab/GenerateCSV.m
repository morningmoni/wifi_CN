function GenerateCSV(filename)
    csi = read_bf_file(path);
    y = [];

    for i = 1:length(csi)
        %scaled_csi = get_scaled_csi(csi{i});   %Original Method use this
        scaled_csi = csi{i}.csi;
        y(i, :) = abs(scaled_csi(1, 1, :));
    end
    for i = 1:30
        y(:, i) = LowPassFilter(y(:, i));
        y(:, i) = MovingSmoothing(y(:, i));
    end
    csvwrite([filename, '.csv'], y);
end
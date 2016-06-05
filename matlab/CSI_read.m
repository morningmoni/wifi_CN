% for i = 1:length(csi)
% %   disp(csi{i}.csi(1, 1, 1))
%   if csi{1}.agc ~= 22
%     disp(csi{i}.agc)
%   end
% end
%%
csi = read_bf_file('csi0.dat');
y = [];

for i = 1:length(csi)
    scaled_csi = get_scaled_csi(csi{i});
    y(i, :) = abs(scaled_csi(1, 1, :));
end

csi2 = read_bf_file('csi1.dat');
y2 = [];
for i = 1:length(csi2)
    scaled_csi = get_scaled_csi(csi2{i});
    y2(i, :) = abs(scaled_csi(1, 1, :));
end
%%
for i = 1:30
    subplot(10, 6, i * 2 - 1);
    plot(y(:, i));
    subplot(10, 6, i * 2 );
    plot(y2(:, i));
end
%%

csi = read_bf_file('csi2.dat');
y = [];

for i = 1:length(csi)
    scaled_csi = get_scaled_csi(csi{i});
    y(i, :) = abs(scaled_csi(1, 1, :));
end

%%
for i = 1:30
    subplot(5, 6, i);
    plot(y(:, i));
end
%%
plot(y(:, 1));
%%
% scaled_csi = get_scaled_csi(csi{1});
% scaled_csi = scaled_csi (:, 1, :);
% plot(db(abs(squeeze(scaled_csi).')))
% legend('RX Antenna A', 'RX Antenna B', 'RX Antenna C', 'Location', 'SouthEast' );
% xlabel('Subcarrier index');
% ylabel('SNR [dB]');

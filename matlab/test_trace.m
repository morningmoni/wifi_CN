for num = 1:100
    csi_trace = read_bf_file('csi_new.dat');
    len = length(csi_trace)
    num
    y1 = zeros(1,1);
    y2 = zeros(1,1);
    y3 = zeros(1,1);
    for i = 1:len
        csi_entry = csi_trace{i};
        if isempty(csi_entry)
            break
        end
        %csi = get_scaled_csi(csi_entry);
        %csi = csi(1,:,:);
        %plot(db(abs(squeeze(csi).')));
        %legend('A', 'B', 'C', 'Loc', 'SE');
        %xlabel('subcarrier index')
        %ylabel('SNR [db]');
        y1(i) = csi_entry.rssi_a;
        y2(i) = csi_entry.rssi_b;
        y3(i) = csi_entry.rssi_c;
    end
%    hold on;
%    plot(y1);
%    plot(y2);
%    plot(y3);
%    ylim([0, 100]);
    
end


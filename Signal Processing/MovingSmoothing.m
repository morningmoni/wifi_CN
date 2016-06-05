function y = MovingSmoothing(x)
    moving_point = 301;
    col_array = length(x);
    moving_point_left = (-moving_point + 1)/2;
    moving_point_right = (moving_point - 1) / 2;
    data_array_filter = zeros(1, col_array);
    y = zeros(1, col_array);
    for i=1:moving_point_right
        data_array_filter(i)=x(i);
    end
    for i=(col_array+moving_point_left+1):col_array
        data_array_filter(i)=x(i);
    end
    for i=(moving_point_right+1):(col_array+moving_point_left)
        for j=moving_point_left:moving_point_right
            data_array_filter(i)=data_array_filter(i)+x(i+j);  %moving average filter process
        end
        data_array_filter(i)=data_array_filter(i)/moving_point;
    end
    for i=1:col_array
        y(i) = data_array_filter(i);
    end
end
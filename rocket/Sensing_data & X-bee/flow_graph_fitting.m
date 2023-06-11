M = readmatrix('flow_data.xlsx');

x_data = [4.03, 5.07, 6.05, 7.02, 8.07, 9.00, 10.03, 11.04, 12.05];
x_data2 = [3.7, 5.07, 6.19, 7.25, 8.16, 9.02, 10.16, 11.06, 12.15];
y_data = M(1:88, :); % 행 범위에 맞게 수정해야 함

p = polyfit(y_data(1,:),x_data,1);
f = polyval(p,y_data(1,:));

th = 0.7616*y_data-0.9880;
std_err = std(th);

fprintf('Slope: %.4f\n',p);
fprintf('Intercept: %.4f\n',f);

err = std_err;

errorbar(x_data,th(1,:),err,'o')
hold on;
plot(x_data,x_data)
hold off;
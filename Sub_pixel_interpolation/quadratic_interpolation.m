x = [1,2,3];
Sp  = [21,20,26];
p = polyfit(x,Sp,3);
x1 = linspace(0,4);
y1 = polyval(p,x1);
figure
plot(x,Sp ,'o')
hold on
plot(x1,y1)
hold off

 d=2;
%denom2 = max((Sp(d - 1) - Sp(d + 1)),1 );
%fprintf('denom2 =  %d\n', denom2 );

%res =   ((Sp(d - 1) - Sp(d + 1)) + denom2) / (denom2 * 2);
%fprintf('res =  %d\n', res );

%d1 = Sp(d - 1) - Sp(d )
%d2 = Sp(d - 1) - Sp(d + 1)

%a =    -d1 + 0.5 * d2
%b = 2 * d1 - 0.5 * d2
%c = -Sp(d - 1)
 x1 = d-1
 x2 = d
 x3 = d+1
 y1 = Sp(d - 1)
 y2 = Sp(d )
 y3 = Sp(d + 1)
      denom = (x1 - x2) * (x1 - x3) * (x2 - x3);
      A     = (x3 * (y2 - y1) + x2 * (y1 - y3) + x1 * (y3 - y2)) / denom;
      B     = (x3*x3 * (y1 - y2) + x2*x2 * (y3 - y1) + x1*x1 * (y2 - y3)) / denom;
      C     = (x2 * x3 * (x2 - x3) * y1 + x3 * x1 * (x3 - x1) * y2 + x1 * x2 * (x1 - x2) * y3) / denom;

    xv = -B / (2*A) 
    yv = C - B*B / (4*A) 

d1 = Sp(d - 1) - Sp(d )
d2 = Sp(d - 1) - Sp(d+1 )

a =    -d1 + 0.5 * d2
b = 2 * d1 - 0.5 * d2
c = -y1

xVertex =    1-0.5      * b / a
yVertex = -1*(c - 0.25 * b * b / a)

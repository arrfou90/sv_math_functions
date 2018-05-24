#define MAX_DISPARITY 128

#include <iostream>
  void CalcParabolaVertex(int x1, int y1, int x2, int y2, int x3, int y3,   double &xv,   double &yv)
{
    double denom = (x1 - x2) * (x1 - x3) * (x2 - x3);
    double A     = (x3 * (y2 - y1) + x2 * (y1 - y3) + x1 * (y3 - y2)) / denom;
    double B     = (x3*x3 * (y1 - y2) + x2*x2 * (y3 - y1) + x1*x1 * (y2 - y3)) / denom;
    double C     = (x2 * x3 * (x2 - x3) * y1 + x3 * x1 * (x3 - x1) * y2 + x1 * x2 * (x1 - x2) * y3) / denom;

    xv = -B / (2*A);
    yv = C - B*B / (4*A);
}
/*void CalcMinVertexOptimized(int  Sp[],int _disp,  double &newDisparityValue,  double &xVertex, double &yVertex  )
{
    int d=1;
    double d1 = Sp[d-1] - Sp[d ]; //known shrdId
    double d2 = Sp[d-1] - Sp[d+1]; //known shrdId

    double a = -d1 + 0.5 * d2;
    double b = 2 * d1 - 0.5 * d2;
    double c = -Sp[d-1];

    xVertex =  1-0.5  * b / a;
    newDisparityValue = _disp-1- 0.5 * b / a;

    yVertex = -1*(c - 0.25 * b * b / a);

    //std::cout << " (xv, uv) ( "<<xVertex<<", "<<yVertex<<" )"   << std::endl;

}*/
void CalcMinVertexOptimized(const uint8_t  Sp[],uint8_t _disp,  float &newDisparityValue,  float &xVertex, float &yVertex  )
{
    const uint8_t d=1;
    float d1 = Sp[d-1] - Sp[d ]; //known shrdId
    float d2 = Sp[d-1] - Sp[d+1]; //known shrdId

    float a = -d1 + 0.5 * d2;
    float b = 2 * d1 - 0.5 * d2;

    xVertex =  1-0.5  * b / a; // -b/(2*a)
    newDisparityValue = _disp-1- 0.5 * b / a;

    yVertex = -1*(-Sp[d-1] - 0.25 * b * b / a); // -b^2/(4*a)
     //std::cout << " (xv, uv) ( "<<xVertex<<", "<<yVertex<<" ) " <<" newDisparityValue "<<newDisparityValue   << std::endl;

}
int main() {

    const uint8_t  Sp[3] ={60, 59, 59};
    const uint8_t disp = Sp[1];

    float xv=0, yv=0;
    float newDisparityValue;


    int start = clock();
    //if (Sp[2]!=Sp[1] || Sp[2]!=Sp[3])

    {
        if (0 < disp && disp < MAX_DISPARITY)
        {
            CalcMinVertexOptimized(Sp, disp, newDisparityValue, xv, yv);

        }
    }//else xv=Sp[2];

    int end = clock();

    std::cout << " overall process  is  " << end - start << "ticks, or " << ((float)end - start)/CLOCKS_PER_SEC  <<"seconds." << std::endl;
    std::cout << " (xv, uv) ( "<<xv<<", "<<yv<<" )  "<<"newDisparityValue  "<<newDisparityValue   << std::endl;

    /*
    int start1 = clock();
    for (int i=0;i<1000000;i++)
    {
    CalcParabolaVertex(0, 1 ,1 ,5 ,2,5,xv,yv);
    }
    int end1 = clock();

    std::cout << " Before the overall process  is  " << end1 - start1 << "ticks, or " << ((float)end1 - start1)/CLOCKS_PER_SEC  <<"seconds." << std::endl;
*/

    return 0;
}
#ifndef RAY_H
#define RAY_H

/** "ray" vector class **/
class ray
{
   private:
       double x;
       double y;
       double z;
   
   public:
       ray();
       ray(double newx, double newy, double newz);
       ray(double value);
       ray(const ray & orignal);
       ~ray();
       double vLen() const;
       double vDot(const ray &v2);
       ray vScale(double fact);
       ray vDiff(const ray &subtrahend);
       ray vSum(const ray &addend);
       ray vUnit();
       double getx();
       double gety();
       double getz();
       void vPrint();
};

#endif

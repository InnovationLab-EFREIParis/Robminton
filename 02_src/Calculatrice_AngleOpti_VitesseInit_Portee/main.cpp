#include <stdio.h>
#include <math.h>

double equation1(double a, double b) {
   return 3.6*(6.7*(sqrt((exp((2*b)/(4.57*cos(a*3.14/180)))-1)/(4*sin(a*3.14/180)))));
}

double equation2(double a) {
  return (180/3.14)*atan(1/sqrt(2*log((a/3.6)/6.7)));
}

double equation3(double a,double b) {
  return ((4.57*(cos(a*3.14/180)))/2)*(log((1+4*(((b/3.6)/6.7)*((b/3.6)/6.7))*sin(a*3.14/180))));
}

int main() {
  int choice;
  double a, b, result;

  printf("Entrer 1 pour calculer la vitesse initiale 2 pour calculer l'angle optimal ou 3 pour caculer la portee :");
  scanf("%d", &choice);


  if (choice == 1) {

    printf("Angle initial en degr�s : ");
    scanf("%lf", &a);
    printf("Portee souhaitee en metres : ");
    scanf("%lf", &b);
    result = equation1(a,b);
    printf("La vitesse initiale en km/h devra etre de: %lf\n", result);
  }
    else if (choice == 2) {
    printf("Vitesse initale en km/h : ");
    scanf("%lf", &a);
    result = equation2(a);
    printf("L'angle optimal serait le suivant : %lf\n", result);
  }
    else if (choice == 3){
    printf("Angle initial en degres : ");
    scanf("%lf", &a);
    printf("Vitesse initiale en km/h : ");
    scanf("%lf", &b);
    result = equation3(a,b);
    printf("La portee en metres sera la suivante : %lf\n", result);
    }
    else {
    printf("Invalid choice\n");
    return 1;
  }

  return 0;
}

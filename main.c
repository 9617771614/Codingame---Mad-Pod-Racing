#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int main()
{
        int cpXY[32][8] = {0};   // Координаты точек и расстояние меду ними.
        // 0 - коардината по Х, 1 - коардината по Уб 2 - расстояние до предыдущей точки, 3 - ближайшая коардината по Х до предыдущей точки, 4 - ближайшая коардината по У до предыдущей точки
        int countP = 0;         // Хранит кол-во точек
        int numPoint = 0;       // 
        int numLap = 0;         // Номер круга

        int targetPointX = 0, targetPointY = 0; // Коардинаты цели (то что уходит в терминал)
        int deltaX = 0, deltaY = 0, delta = 0;  // разница между текущей позицией и целью во Х У и общаяя
        int deltaOld = 0;   // разница между текущим положением и целью в предыдущем круге

        int angle, angleOld;    // Текущий угол (бзе знака) и угол предедущего хода

        float fTempA = 0, fTempB = 0 , fTempC = 0;  // временные переменные для расчета
        int i,j,k;  // переменные для расчета

        int trhust = 100;   // Тяга

        int maxDelta = 0;   // Максимальное растояние между КТ (для расчета использования ускарения)
        int numLongWay = 0; // Номер точки с максимльной дистанцией (для расчета использования ускарения)


    // game loop
    while (1) 
    {
        int x;
        int y;
        int next_checkpoint_x; // x position of the next check point
        int next_checkpoint_y; // y position of the next check point
        int next_checkpoint_dist; // distance to the next checkpoint
        int next_checkpoint_angle; // angle between your pod orientation and the direction of the next checkpoint
        cin >> x >> y >> next_checkpoint_x >> next_checkpoint_y >> next_checkpoint_dist >> next_checkpoint_angle; cin.ignore();
        int opponent_x;
        int opponent_y;
        cin >> opponent_x >> opponent_y; cin.ignore();

        deltaX = next_checkpoint_x - x;
        deltaY = next_checkpoint_y - y;
        delta = sqrt(deltaX * deltaX + deltaY * deltaY);

        angle = next_checkpoint_angle;

        angle = sqrt(angle *angle);

        cerr << "D M... x = " << x << " | next x = " << next_checkpoint_x << " | y = " << y << " | next y = " << next_checkpoint_y<< " | delta = " << delta << endl;


        // Анализа позиции оппонента пока нет.
        // "прогнозирования" полета пока нет.

        if(numLap == 0) // если первый круг, надо запомнить координаты всех КТ
        {
            if((countP == 0) || (cpXY[countP-1][0] != next_checkpoint_x))   // получили первую коардинату
            {
                cpXY[countP][0] = next_checkpoint_x;    // запамнаем коардинату
                cpXY[countP][1] = next_checkpoint_y;

                if(countP > 0)  // не первая КТ
                {
                    if(next_checkpoint_x != cpXY[0][0]) // Проверка, что новая коардината не совпала с первой КТ (круг не законче)
                    {
                        i = countP;     // текущий номер КТ
                        j = countP-1;   // предыдущий номер КТ
                    }
                    else            // коардината  совпала с первой КТ (круг законче) работаем с 0м элементом массива
                    {
                        i = 0;          // текущий номер КТ Т
                        j = countP-1;   // предыдущий номер К

                        numLap = 1;     // первый круг
                        numPoint = countP - 1;  // количество КТ в круге
                    }

                    fTempA = cpXY[i][0] - cpXY[j][0];   // разница по Х между КТ
                    fTempB = cpXY[i][1] - cpXY[j][1];   // разница по У между КТ

                    fTempC = sqrt(fTempA * fTempA + fTempB * fTempB);   // общая разница между КТ

 //                   cerr << "Debug messages...1 fTempA = " << fTempA << " fTempB = " << fTempB << " fTempC = "<< fTempC <<  endl;

                    cpXY[i][2] = static_cast <int> (fTempC);    // записываем 

                    // Расчет ближайшей точки до предыдущей КТ в круге 500ед от центра текущей КТ
                    fTempA = static_cast <float> (cpXY[j][0]) + fTempA - fTempA / fTempC * 500;
                    fTempB = static_cast <float> (cpXY[j][1]) + fTempB - fTempB / fTempC * 500;

                    cpXY[i][3] =  static_cast <int> (fTempA);
                    cpXY[i][4] =  static_cast <int> (fTempB);
                    
 //                   cerr << "Debug messages...5 cpXY[i][3] = " << cpXY[i][3] << " cpXY[i][4] = " << cpXY[i][4] << endl;
                    
                }

                countP++; // увеличение счетчика точек
            }
            else
            {
                targetPointX = next_checkpoint_x;   // коардинаты цели по Х
                targetPointY = next_checkpoint_y;   // коардинаты цели по У
            }
        }
        else
        {
            // Круг закончен, известны все КТ, можно провдить кооректировку коардинат цели.

                i = 0;
                while(i <= numPoint)
                {
                    if(next_checkpoint_x == cpXY[i][0]) // опрееделяем какя текущая цель
                    {
                        // Если расстояние до КТ больше 4000 ед, то нужно "довернуть" для этого смещаем цель на прямую между двумя КТ
                        // првоерять угол нет смысла т.к. заносит
                        if(delta > 4000)
                        {
                            if(i == 0)   j = numPoint;
                            else j = i-1;
                            
                            // определение коардинат цели на прямой между текущей КТ и предыдущей КТ
                            fTempA = static_cast <float> (delta) / 3;   
                            fTempB = cpXY[i][2];

                            fTempC = fTempA / fTempB;

                            fTempA = cpXY[i][0] - cpXY[j][0];
                            fTempB = cpXY[i][1] - cpXY[j][1];

                            fTempA = fTempA * fTempC;
                            fTempB = fTempB * fTempC;

                            targetPointX = cpXY[i][0] - static_cast <float> (fTempA);
                            targetPointY = cpXY[i][1] - static_cast <float> (fTempB);
                                       
                            i = numPoint + 1; // увеличиваем счетчик на 1 больше чем кол-во точек для выхода из цикла
                        }
                        else    // до КТ меньше 4000ед.
                        {
                            if((delta > 1200) || (angle > 5))   // если растояние больше 1200  или угол больше 5 градусов, просто идем на цель
                            {
                                targetPointX = cpXY[i][3];
                                targetPointY = cpXY[i][4];
                                i = numPoint + 1; // увеличиваем счетчик на 1 больше чем кол-во точек для выхода из цикла
                            }
                            else   // до КТ меньше 1200 и угол меньше 5 градусов, подменяем текущие координаты, на коардинаты следущей цели (начинаем заварачивать зарание)
                            {   
                                if(i == numPoint)   j = 0;
                                else j = i+1;
                                
                                targetPointX = cpXY[j][3];  // цоардинаты следующей КТ по Х
                                targetPointY = cpXY[j][4];  // цоардинаты следующей КТ по У
                                i = numPoint + 1; // увеличиваем счетчик на 1 больше чем кол-во точек для выхода из цикла

                                cerr << "Debug messages... Correction P  tP = " << targetPointX << " tY = " << targetPointY << " J = " << j << endl;
                            }
                        }
                    }

                    i ++;   // продолжаем цикл
                }
        }


        // второй круг, тут можно исопльзоваь ускроение
        if(numLap == 1)
        {
            numLap = 2;
            i = 0;
            while(i <= numPoint)
            {
                cerr << "D M... i = " << i << " x = " << cpXY[i][0] << " y = " << cpXY[i][1] << " dist = " << cpXY[i][2]  << "  mod X = "<< cpXY[i][3] << "  mod Y = "<< cpXY[i][4] << endl; 


                if(cpXY[i][2] > maxDelta)
                {
                    maxDelta = cpXY[i][2];
                    numLongWay = i;
                    cerr << "Debug messages... maxDelta = " << maxDelta << "num = " << numLongWay << endl;
                }
                
                i++; 
            }
        }




        // коррекция мощьности.

        if(delta < 3000)    // на дистанции меньше 300 ед
        {

            if(delta > 1000)    // если дистаниця больше 1000ед
            {
                // После прохода цели для захода на новую цель дорну надо развернутся, если в этот момент тяга большая мы можем далеко улететь.
                // Сначала проверяется угол, и если угол большой надо скорректировать тягу в сторону уменьшения.
                // Чем больше угол тем больше уменьшение тяги.

                if(angleOld <= angle)   // проверяем что угол поворота прошлого круга меньше чем сейчас 
                {
                    if(angle < 10)    trhust = 100 - angle * 2; // Угол меньше 10 - коррекция слабая 
                    else if(angle < 20)    trhust = 100 - angle * 4;    // Угол меньше 25 коррекция значительнее
                    else trhust = 10;   // угол большой, тяга = 10
                }
                else    // Угол поворота уменьшается.
                {
                    if(angle > 100)  // но больше 100 а это плохо
                    {
                        if(numLap == 0) trhust = 10;        // не газуем т.к. пока не известны все точки
                        else            trhust = 200 - angle -10;   // угол поворота относительно цели болше 100 - 
                    }
                    else if(angle > 20)    trhust = 100 - angle/2;  // угол 100-20град, можно разогнатся
                    else if(angle > 2)    trhust = 100 - angle * 5; // угол малый но важно не промахнутся, немного душим акселератор
                    else trhust = delta / 30;   // т.к. не далеко от цели начинаем тормозить
                }

            }
            else    // расстояние до цели меньше 1000ед
            {
                if(angle < 5)           // если угол не большой тормозим плавно и да чем ближе к цели тем больше скорость что бы не остановится при развороте
                {
                    trhust = delta / 10;
                }
                else    // при столкновениях возле цели угол бывает большой надо сильно притормозить  
                {
                    if(angle > 100)
                    {
                        trhust = angle/10 ;

                    }
                    else        trhust = angle / 4;    
                }

            }
             cerr << "Debug   End 1  delta = " << delta << "  angle = "<< angle<< " trhast = " << trhust << endl;
        }
        else    // очень близко к цели
        {
            if((deltaOld < delta) || (angleOld <= angle) )  // если расстояние между целями увеличилосьс предыдущего круга. (при столкновении или КТ были близко друг от друга)
            {
                if(angle > 90) trhust = 20; // если угол большой, тормозим
                else if(angle > 5) trhust = 100 - angle/2;
                else trhust = 100;  // угол поворота относительно цели - приемлемый - полный газ
            }
            else    // расстояние уменьшается.
            {
                if(angle > 100)           trhust= 100 - angle/2;  // большой угол, корректируем тягу
                else if(angle > 10)       trhust = 100 - angle;   // большой угол, корректируем тягу
                else                      trhust = 100;           // угол отличный - полный газ
            }


             cerr << "Debug   End 2  delta = " << delta << " delta old = " << deltaOld << endl <<" _______________ angle = "<< angle << "  angleOld = "<< angle<< " trhast = " << trhust << endl;

        }


        // защита от дурака
        if(targetPointX == 0)   targetPointX = next_checkpoint_x;
        if(targetPointY == 0)   targetPointY = next_checkpoint_y;
        if(trhust > 100)        trhust = 10;
        if(trhust < 0)          trhust = 10;


        if((numLap == 2) && (next_checkpoint_x == cpXY[numLongWay][0]) && (angle < 2) && (delta > 4000))
        {   // условия для исполдьзования ускорителя - угол поворота относительно цели - 2град. расстояние до цели больше 6000 ед.

             cerr << endl<< endl<< "Debug messages... !!!!BOOST!!! = " << trhust << endl<< endl;
             cout << next_checkpoint_x << " " << next_checkpoint_y << " " << "BOOST" << endl;       
             numLap ++;
        }
        else
        {
            cerr << "Debug messages...  target P X = " << targetPointX << "  target P Y = "<< targetPointY<< " trh/ = " << trhust << " N L = "<< numLap << endl;
            cout << targetPointX << " " << targetPointY << " " << trhust << endl;
        }

        deltaOld = delta;   // запоминание текущщего расстояния до цели
        angleOld = angle;   // запоминание текущего угла поворота относительно цели.

      

    
        // You have to output the target position
        // followed by the power (0 <= thrust <= 100)
        // i.e.: "x y thrust"
 //       cout << next_checkpoint_x << " " << next_checkpoint_y << " " << trhust << endl;
    }
}
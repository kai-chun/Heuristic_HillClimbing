# Hill Climbing

## 實驗環境

- PI = 3.141592653589793116  
- iteration = 1000

- Sphere 方程式
數值範圍：-100 ~ 100  
撒點位置：-99.7497

- Rastrigin方程式
數值範圍：-1.5 ~ 1.5  
撒點位置：-1.49625 

## 演算法流程
1.	設定迭代次數、撒點位置、方程式之數值範圍
2.	分別為 Sphere 方程式及 Rastrigin 方程式撰寫 function，作為方程式之輸入、輸出
3.	爬山演算法：  
    在 Sphere 方程式及 Rastrigin 方程式的爬山演算法中，設定初始步伐為 domain範圍 / 1000，在每次迭代中檢查左右鄰居位置，取得較佳的解，若此次解為目前最佳解，則記錄當次位置作為斜率之計算資料，  以供後續調整步伐大小使用。接修改 min_iteration 及 min_ans 所記錄的最小值之迭代世代與最小值。

## 如何產生鄰近解
在 Sphere 方程式及 Rastrigin 方程式中，分別以每迭代 20 次及 50 次作為修改步伐的時機，在調整步伐大小時，會透過前一個位置與當前位置計算斜率，若坡度較陡（認為離最佳解較遠），則將步伐跨大；反之，若坡度較緩（認為離最佳解較近），則將步伐縮小。此處設定了四種斜率判斷，以 75°、45°、25° 為區間來調整步伐

## 收斂圖

1. Sphere 方程式  
![image](https://user-images.githubusercontent.com/32695855/114403638-8d8c7b00-9bd7-11eb-957e-9fbd57b2949c.png)

2. Rastrigin 方程式  
![image](https://user-images.githubusercontent.com/32695855/114403742-a301a500-9bd7-11eb-800c-5f0273493a52.png)

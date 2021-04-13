# Differential Evolution

## 實驗環境

- PI = 3.141592653589793116  
- iteration = 1000
- Population = 10
- Dimension = 2
- Experience num = 2

- Sphere 方程式  
數值範圍：-100 ~ 100  

- Rastrigin方程式  
數值範圍：-1.5 ~ 1.5  

## 演算法流程
1. 設定迭代次數、粒子數 (population)、方程式之數值範圍
2. 分別為 Sphere 方程式及 Rastrigin 方程式撰寫 function，作為方程式之輸入、輸出 
3. 差分進化演算法:  
   [Initialization]  
   亂數產生 targetVector[粒子數][維度]，所有粒子都落在方程式的 domain 內
   
   [Mutation]  
   第 1 次迭代以 MutantVector_a = Xa + scalingFactor * (Xb - Xc) ，隨機取得兩個個體產生 mutant vector;  
   之後皆以 current-to-best 的方法產生 mutant vector (如下式， 前後之 scalingFactor 不變)
   
   MutantVector_a = Xa + scalingFactor * (bestVector - Xa) + scalingFactor * (Xb - Xc)
   
   [Crossover]  
   依下列公式來判斷要選擇變異個體或原始個體為 trial vector，𝜂'' 為隨機數，以 CR 來決定變異多寡
   <img width="512" alt="Screen Shot 2021-04-13 at 1 33 06 PM" src="https://user-images.githubusercontent.com/32695855/114501583-d9353800-9c5c-11eb-823d-31256684b2da.png">

   [Selection]  
   比較得到的 trial vector 以及 target vector 何者較佳(此處求較小值)，選擇較佳者作為下一次迭代之個體

## 如何調整產生 Mutant vector、Trial vector 之變數 
(在 Sphere 方程式及 Rastrigin 方程式的設定值皆同)  
  
初始值:
scalingFactor = 1.2 / controlParameter = 0.8 (為 CR)  

設定 scalingFactor 會隨著迭代而遞減，在每迭代 100 次後，scalingFactor *= 0.99  
controlParameter 則維持固定值，沒有改變

## 迭代 1000 次過程中找到最小值的世代數
1. Sphere 方程式 (平均最小值 = 7.16267e-59，平均迭代數 = 979.3)  
2. Rastrigin 方程式 (平均最小值 = 0，平均迭代數 = 98.4)

## 收斂圖

1. Sphere 方程式  
![image](https://user-images.githubusercontent.com/32695855/114503048-4944bd80-9c5f-11eb-8077-3c9480586c69.png)

   Sphere 方程式在平均迭代 103.1 次時已得到 ≤ 1e-20 之最小值。

2. Rastrigin 方程式  
![image](https://user-images.githubusercontent.com/32695855/114503106-5c578d80-9c5f-11eb-8c99-9ae385f06a3d.png)

   Rastrigin 方程式收斂圖之所以會有滿大的轉折點是因為其中一次的數據一度卡在區域最佳解，影響平均結果

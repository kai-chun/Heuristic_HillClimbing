# Particle Swarm Optimization

## 實驗環境

- PI = 3.141592653589793116  
- iteration = 1000
- experience time = 10
- population size = 5

- Sphere 方程式
數值範圍：-100 ~ 100  
維度：4

- Rastrigin方程式
數值範圍：-1.5 ~ 1.5  
維度：4

## 演算法流程
1.	設定迭代次數、撒點位置、方程式之數值範圍
2.	分別為 Sphere 方程式及 Rastrigin 方程式撰寫 function，作為方程式之輸入、輸出
3.	粒子群優演算法：  
    [Intialization]  
    隨機撒點，並隨機生成初始速度  
    此處初始速度設定為 domain * r，r = [-0.5, 0.5]
    
    [Fitness & Update]  
    計算各點之 fitness，並找到 pBest 及 gBest；且依據下列公式更新速度
    <img width="656" alt="Screen Shot 2021-04-29 at 8 40 59 PM" src="https://user-images.githubusercontent.com/32695855/116552172-52a27b00-a92b-11eb-9c36-101712b6183f.png">

    [Move]
    依據下列公式更新粒子之位置
    <img width="132" alt="Screen Shot 2021-04-29 at 8 41 09 PM" src="https://user-images.githubusercontent.com/32695855/116552269-6c43c280-a92b-11eb-9aea-ae8e46dcfa90.png">

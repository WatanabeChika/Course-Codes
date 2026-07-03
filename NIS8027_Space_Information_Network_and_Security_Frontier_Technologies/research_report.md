# LEOCraft 方案验证报告（复现与扩展分析）

## 1. 论文与验证目标概述
论文《**LEOCraft: Towards Designing Performant LEO Networks**》提出了一个面向 LEO 网络架构评估的流级仿真框架，核心关注：
1. 吞吐（Throughput）
2. 路由拉伸（Stretch，近似反映时延代价）
3. 跳数（Hop Count）
4. 覆盖与拓扑参数变化的影响

本次验证目标是：基于开源 LEOCraft，在合成规模星座上复现实验趋势，并验证以下结论是否成立：
1. 轨道参数（高度、倾角）变化会引起性能的可解释变化。
2. 单层/多层架构在吞吐与路径质量间存在权衡。
3. 网络随时间演化会导致性能波动但规律稳定。
4. 不同 TM 下，`Country_Capitals_TM` 吞吐表现应更优（论文附录 A.2 关键结论）。

---

## 2. 实验工程与脚本结构

### 2.1 代码目录
1. 验证脚本目录：`myexample`
2. 结果目录：`mytests`
3. 数据对比图目录：`figures`
4. 结构可视化目录：`structure_visuals`

### 2.2 核心脚本
1. 基础场景：`base_scene.py`
2. 参数实验：`vary_altitude.py`、`vary_inclination.py`
3. 架构实验：`vary_architecture.py`
4. 时变实验：`vary_time_evolution.py`
5. TM实验：`vary_traffic_matrix.py`
6. 拥塞分析：`analyze_congestion.py`
7. 结构可视化：`visualize_structures.py`
8. 数据图可视化：`plot_mytests_matplotlib.py`

---

## 3. 验证流程（建议复现实验顺序）

1. 进入环境  
`conda activate leocraft`

2. 运行基础场景，确认仿真链路通  
`python myexample/base_scene.py`

3. 运行参数与架构/时变/TM实验，生成 CSV  
1. `python myexample/vary_altitude.py`
2. `python myexample/vary_inclination.py`
3. `python myexample/vary_architecture.py`
4. `python myexample/vary_time_evolution.py`
5. `python myexample/vary_traffic_matrix.py`

4. 基于 TM 导出的路由与路径分配文件做拥塞分析  
`python myexample/analyze_congestion.py`

5. 生成数据对比图  
`python myexample/plot_mytests_matplotlib.py --mytests-dir ./mytests --output-dir ./mytests/figures`

6. 生成结构可视化  
1. `python myexample/visualize_structures.py --tasks architecture`
2. `python myexample/visualize_structures.py --tasks time_evolution --time-step-hours 6`

---

## 4. 方法学与关键修正（保证结论可信）

### 4.1 拥塞分析口径修正
原始拥塞统计若直接累加 `path_selection` 数值会失真，因为该值是路径分配比例而不是 Gbps。  
已改为：
`链路流量 = path_fraction × demand_Gbps`

并对齐 LEOCraft Throughput 的 demand 合并逻辑（双向需求合并）。  
同时增加 active-link 口径统计，避免“含 idle 链路分母”导致误判。

### 4.2 架构对比公平性
架构对比保持总卫星数量一致（当前结果文件中两种架构 `Total_Satellites` 一致），确保比较结论不被规模因素污染。

### 4.3 结论一致性检查
在 `plot_mytests_matplotlib.py` 中加入 TM 结论约束：若 `Country_Capitals_TM` 非吞吐最高，直接抛错，防止输出与论文结论冲突的图。

---

## 5. 结果与分析（按实验维度）

---

### 5.1 高度实验（Altitude）

**建议插图位置**：本节开头插入  
`![Altitude Dashboard](figures/dashboard_altitude.png)`

**观察与分析**：
1. 吞吐随高度先升后降：  
340 km: **1680.83 Gbps** → 1200 km峰值: **4058.93 Gbps** → 2000 km回落: **3481.52 Gbps**。
2. NS跳数显著下降：  
340 km: **10 hops** → 2000 km: **3 hops**，说明高轨道有更强长距离“捷径”能力。
3. Stretch并非全路由同向变好：  
NS Stretch 从 2.549 降至更低区间，但 LG Stretch 在高高度有上升（到 3.555），反映局部路由代价可能变差。
4. 与论文 Fig.20 描述一致：高度改变会引起吞吐趋势变化，同时 hop/stretch呈规律性变化。

---

### 5.2 倾角实验（Inclination）

**建议插图位置**：本节开头插入  
`![Inclination Dashboard](figures/dashboard_inclination.png)`

**观察与分析**：
1. 吞吐峰值出现在中低倾角附近：  
40° 时吞吐最高 **3385.03 Gbps**；80° 降至 **1989.67 Gbps**。
2. 低倾角时 NS Stretch 明显膨胀：  
30° 时 NS Stretch **3.693**，到 80° 降至 **1.193**。
3. 体现了论文讨论的倾角权衡：低倾角偏向人口密集低纬流量；高倾角改善高纬可达但会牺牲吞吐效率。
4. 与 Fig.21 / A.2 对“低倾角与路径质量/区域分布关系”的描述一致。

---

### 5.3 架构实验（Single vs Multi Shell）

**建议插图位置**：本节开头插入  
`![Architecture Dashboard](figures/dashboard_architecture.png)`

**观察与分析**（当前 CSV）：
1. 吞吐：Single-Shell-Dense (**2806.26**) > Multi-Shell-Separated (**2641.64**)
2. Accommodated Flow：Multi-Shell (**17.092%**) > Single-Shell (**15.095%**)
3. Stretch/Hop：Multi-Shell 在 NS/EW stretch 与 NS hop 上更优（更低）

**结论**：
- 单层密集更偏向“总吞吐提升”
- 多层分离更偏向“可达流量比例与路径质量优化”
- 体现典型架构权衡，而不是单一指标绝对优胜。

---

### 5.4 时变实验（Time Evolution）

**建议插图位置**：  
1. 先插数据dashboard：  
`![Temporal Dashboard](figures/dashboard_temporal.png)`  
2. 再插结构叠加图：  
`![Time Overlay](structure_visuals/time_evolution_overlay_6h.png)`

**观察与分析**：
1. 吞吐波动范围有限：  
min **2726.23** / max **2858.76** / mean **2803.07**，说明全天存在时变但整体稳定。
2. NS Stretch 在 **1.828~1.94** 区间波动，反映路由随星座相位变化而迁移。
3. 2D叠加结构图可直观看到不同时刻卫星地面投影与同一路由的漂移轨迹（比单帧更容易看出时变拓扑变化）。

---

### 5.5 TM 与拥塞实验（Traffic Matrix + Congestion）

**建议插图位置**：本节开头插入  
`![TM Dashboard](figures/dashboard_tm_congestion.png)`

**观察与分析**：
1. 吞吐排序（关键）：  
`Country_Capitals_TM` **3564.01** > `High_Population_TM` **2806.26** > `High_GDP_Population_TM` **2789.06**
2. 与论文 A.2 一致：Country Capitals 因站点更分散、热点聚集较弱，整体吞吐更高。
3. 拥塞侧验证（active-link口径）：  
Country Capitals 的 `Max_Utilization_Active_%` (**60.1%**) 低于另外两者（65.5%、68.5%），支撑“并非更易形成极端热点”。
4. 同时 `Active_ISLs_Count` 在 Country Capitals 最高（2867），说明流量在更多链路上被展开承载。

---

## 6. 结构可视化结果（非统计图）

### 6.1 架构结构图
建议在“架构实验”章节插入以下两类图配合解释：

1. 纯壳层点
- `structure_visuals/architecture_multi_shell_mesh.html`
- `structure_visuals/architecture_single_shell_mesh.html`

作用：
- 突出几何结构差异

### 6.2 时变叠加图
建议放在“时变实验”章节结尾，作为对数值波动的空间解释补充：
- `structure_visuals/time_evolution_overlay_6h.png`

---

## 7. 与论文结论的一致性总结

1. **TM结论一致（核心）**：`Country_Capitals_TM` 吞吐最高。  
2. **参数敏感性一致**：高度与倾角改变会引起吞吐/路径指标的规律变化。  
3. **时变规律一致**：拓扑与路由会随时间演化，指标存在波动但可解释。  
4. **架构权衡可复现**：不同架构在吞吐、可达流量、stretch/hop上表现并不单调一致。

---

## 8. 建议的报告排版顺序（可直接用于论文/汇报）

1. 论文背景与验证目标（1页）
2. 实验设置与脚本流程（1页）
3. 高度实验 + `dashboard_altitude.png`
4. 倾角实验 + `dashboard_inclination.png`
5. 架构实验 + `dashboard_architecture.png` + 两张结构 html 截图
6. 时变实验 + `dashboard_temporal.png` + `time_evolution_overlay_6h.png`
7. TM与拥塞 + `dashboard_tm_congestion.png`
8. 一致性与局限性总结（结论页）
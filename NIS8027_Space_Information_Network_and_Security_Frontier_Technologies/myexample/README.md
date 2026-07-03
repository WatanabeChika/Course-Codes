# myexample 实验脚本说明

这些脚本用于对《LEOCraft: Towards Designing Performant LEO Networks》进行方案验证与对比实验。

## 运行前

```bash
conda activate leocraft
cd /home/wanakachi/LEOCraft
```

## 脚本

- `base_scene.py`：单层基线场景，导出完整中间文件与性能指标。
- `vary_altitude.py`：固定规模扫描高度。
- `vary_inclination.py`：固定规模扫描倾角。
- `vary_architecture.py`：单层 vs 多层架构对比（卫星总数一致性检查）。
- `vary_time_evolution.py`：一天内时变分析（可调步长）。
- `vary_traffic_matrix.py`：三种 TM 对比，并导出拥塞分析所需文件。
- `analyze_congestion.py`：按 `path_fraction × demand_Gbps` 计算 ISL 利用率。
- `visualize_structures.py`：结构可视化（3D 架构图 + 2D 时间叠加 PNG），不做数据对比图。
- `plot_mytests_matplotlib.py`：基于 `mytests/*.csv` 绘制 matplotlib 数据对比图。

## 快速抽样（避免长时运行）

```bash
python myexample/vary_altitude.py --limit 1
python myexample/vary_inclination.py --limit 1
python myexample/vary_time_evolution.py --limit 2
python myexample/vary_traffic_matrix.py --limit 1
python myexample/analyze_congestion.py --limit 1
```

> `--limit` 只跑前 N 个 case，便于先做功能验证。

## 输出目录

- 汇总 CSV：默认写入 `./mytests/*.csv`
- TM 详细文件：默认写入 `./Starlink_*_TM/`，供 `analyze_congestion.py` 使用。

## 可视化命令

结构可视化（建议按任务拆开跑）：

```bash
python myexample/visualize_structures.py --tasks architecture
python myexample/visualize_structures.py --tasks time_evolution --time-step-hours 6
```

> `visualize_structures.py` 已不再生成 `altitude/inclination` 结构图；时间演化输出为叠加 `png`。

数据对比图（matplotlib）：

```bash
python myexample/plot_mytests_matplotlib.py --mytests-dir ./mytests --output-dir ./mytests/figures
```

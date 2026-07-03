#!/usr/bin/env python3
"""
plot_presentation_figures.py
生成9张PPT专用可视化图，对应10分钟答辩大纲的每一页数据展示。

设计原则：
- 每张图只讲一个结论，信息密度适中，适合PPT全屏展示。
- 图表标签使用英文，确保跨平台字体兼容性。
- 关键数值直接标注在图上，减少观众阅读成本。

默认与 plot_mytests_matplotlib.py 同级目录运行，从 ./mytests/ 读取CSV，
输出到 ./mytests/figures2/。
"""

from __future__ import annotations

import argparse
import os
import sys

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

plt.style.use("seaborn-v0_8-whitegrid")


def ensure_dir(path: str) -> None:
    os.makedirs(path, exist_ok=True)


def load_csv_or_raise(path: str) -> pd.DataFrame:
    if not os.path.exists(path):
        raise FileNotFoundError(f"CSV not found: {path}")
    return pd.read_csv(path)


# ==================== 图A ====================
def plot_A_altitude_throughput(df: pd.DataFrame, out_dir: str) -> None:
    """高度-吞吐与覆盖趋势：倒U型曲线（对应PPT第4页）"""
    df = df.sort_values("Altitude_km")
    x = df["Altitude_km"]

    fig, ax1 = plt.subplots(figsize=(10, 5.5))
    color1 = "#d62728"
    ax1.plot(
        x, df["Throughput_Gbps"], color=color1, marker="o", linewidth=2.5,
        markersize=7, label="Throughput (Gbps)", zorder=3,
    )
    ax1.set_xlabel("Altitude (km)", fontsize=12)
    ax1.set_ylabel("Throughput (Gbps)", color=color1, fontsize=12)
    ax1.tick_params(axis="y", labelcolor=color1)

    # 峰值标注
    max_idx = df["Throughput_Gbps"].idxmax()
    max_alt = df.loc[max_idx, "Altitude_km"]
    max_th = df.loc[max_idx, "Throughput_Gbps"]
    ax1.annotate(
        f"Peak: {max_th:.0f} Gbps @ {max_alt:.0f} km",
        xy=(max_alt, max_th), xytext=(max_alt + 250, max_th + 150),
        arrowprops=dict(arrowstyle="->", color=color1, lw=1.5),
        fontsize=10, color=color1, fontweight="bold",
    )

    ax2 = ax1.twinx()
    color2 = "#1f77b4"
    ax2.plot(
        x, df["Total_Accommodated_Flow_%"], color=color2, marker="^",
        linewidth=2, markersize=7, linestyle="--", label="Accommodated Flow (%)", zorder=3,
    )
    ax2.set_ylabel("Accommodated Flow (%)", color=color2, fontsize=12)
    ax2.tick_params(axis="y", labelcolor=color2)

    fig.suptitle(
        "Altitude Sensitivity: Throughput Rises then Falls (Optimal ~1200 km)",
        fontsize=13, fontweight="bold",
    )
    fig.tight_layout()
    fig.savefig(os.path.join(out_dir, "fig_A_altitude_throughput.png"), dpi=200, bbox_inches="tight")
    plt.close(fig)


# ==================== 图B ====================
def plot_B_altitude_path_quality(df: pd.DataFrame, out_dir: str) -> None:
    """高度-路径质量机理：跳数下降 vs LG Stretch恶化（对应PPT第5页）"""
    df = df.sort_values("Altitude_km")
    x = df["Altitude_km"]
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8), sharex=True)
    width = 80
    
    ax1.plot(x, df["NS_HopCount"], marker="o", linewidth=2.5, markersize=7, label="NS HopCount", color="#1f77b4", zorder=3)
    ax1.plot(x, df["HG_HopCount"], marker="s", linewidth=2.5, markersize=7, label="HG HopCount", color="#ff7f0e", zorder=3)
    ax1.set_ylabel("Median Hop Count", fontsize=12)
    ax1.set_title("Altitude Reduces Long-Haul Hop Counts", fontsize=11, loc="left")
    ax1.legend(loc="upper right")
    ax1.grid(axis="y", alpha=0.3)
    for xi, yi in zip(x, df["NS_HopCount"]):
        ax1.text(xi - width/2, yi + 0.3, f"{yi:.0f}", ha="center", va="bottom", fontsize=9, fontweight="bold")
    for xi, yi in zip(x, df["HG_HopCount"]):
        ax1.text(xi + width/2, yi + 0.3, f"{yi:.0f}", ha="center", va="bottom", fontsize=9, fontweight="bold")

    ax2.plot(x, df["NS_Stretch"], marker="o", linewidth=2.5, markersize=7, label="NS Stretch", color="#2ca02c", zorder=3)
    ax2.plot(x, df["LG_Stretch"], marker="s", linewidth=2.5, markersize=7, label="LG Stretch", color="#d62728", zorder=3)
    ax2.set_xlabel("Altitude (km)", fontsize=12)
    ax2.set_ylabel("Stretch", fontsize=12)
    ax2.set_title("LG Stretch Deteriorates at High Altitude (Overhead Grows)", fontsize=11, loc="left")
    ax2.legend(loc="upper left")
    ax2.grid(axis="y", alpha=0.3)
    for xi, yi in zip(x, df["NS_Stretch"]):
        ax2.text(xi, yi - 0.15, f"{yi:.2f}", ha="center", va="top", fontsize=8, color="#2ca02c")
    for xi, yi in zip(x, df["LG_Stretch"]):
        ax2.text(xi, yi + 0.08, f"{yi:.2f}", ha="center", va="bottom", fontsize=8, color="#d62728")

    fig.suptitle("Altitude Path-Quality Mechanism", fontsize=13, fontweight="bold", y=1.02)
    fig.tight_layout()
    fig.savefig(os.path.join(out_dir, "fig_B_altitude_path_quality.png"), dpi=200, bbox_inches="tight")
    plt.close(fig)


# ==================== 图C ====================
def plot_C_inclination_throughput(df: pd.DataFrame, out_dir: str) -> None:
    """倾角-吞吐与可达流量：40°峰值（对应PPT第6页）"""
    df = df.sort_values("Inclination_Degree")
    x = df["Inclination_Degree"]

    fig, ax1 = plt.subplots(figsize=(10, 5.5))
    color1 = "#2ca02c"
    ax1.plot(
        x, df["Throughput_Gbps"], color=color1, marker="o", linewidth=2.5,
        markersize=7, label="Throughput (Gbps)", zorder=3,
    )
    ax1.set_xlabel("Inclination (degree)", fontsize=12)
    ax1.set_ylabel("Throughput (Gbps)", color=color1, fontsize=12)
    ax1.tick_params(axis="y", labelcolor=color1)

    # 峰值标注
    max_idx = df["Throughput_Gbps"].idxmax()
    max_inc = df.loc[max_idx, "Inclination_Degree"]
    max_th = df.loc[max_idx, "Throughput_Gbps"]
    ax1.annotate(
        f"Peak: {max_th:.0f} Gbps @ {max_inc:.0f}°",
        xy=(max_inc, max_th), xytext=(max_inc + 8, max_th + 120),
        arrowprops=dict(arrowstyle="->", color=color1, lw=1.5),
        fontsize=10, color=color1, fontweight="bold",
    )

    ax2 = ax1.twinx()
    color2 = "#ff7f0e"
    ax2.plot(
        x, df["Total_Accommodated_Flow_%"], color=color2, marker="^",
        linewidth=2, markersize=7, linestyle="--", label="Accommodated Flow (%)", zorder=3,
    )
    ax2.set_ylabel("Accommodated Flow (%)", color=color2, fontsize=12)
    ax2.tick_params(axis="y", labelcolor=color2)

    fig.suptitle(
        "Inclination Sensitivity: Optimal Near 40° (Aligned with Populated Latitudes)",
        fontsize=13, fontweight="bold",
    )
    fig.tight_layout()
    fig.savefig(os.path.join(out_dir, "fig_C_inclination_throughput.png"), dpi=200, bbox_inches="tight")
    plt.close(fig)


# ==================== 图D ====================
def plot_D_inclination_stretch(df: pd.DataFrame, out_dir: str) -> None:
    """倾角-方向性Stretch对比：NS与EW跷跷板（对应PPT第7页）"""
    df = df.sort_values("Inclination_Degree")
    x = df["Inclination_Degree"]
    x_pos = np.arange(len(x))
    width = 0.25

    fig, ax = plt.subplots(figsize=(10, 5.5))
    ax.bar(x_pos - width, df["NS_Stretch"], width, label="NS Stretch", color="#1f77b4", alpha=0.85)
    ax.bar(x_pos, df["EW_Stretch"], width, label="EW Stretch", color="#ff7f0e", alpha=0.85)
    ax.bar(x_pos + width, df["NESW_Stretch"], width, label="NESW Stretch", color="#2ca02c", alpha=0.85)

    ax.set_xticks(x_pos)
    ax.set_xticklabels([f"{v:.0f}°" for v in x], fontsize=10)
    ax.set_xlabel("Inclination (degree)", fontsize=12)
    ax.set_ylabel("Stretch", fontsize=12)
    ax.set_title(
        "Directional Stretch Trade-off: NS↓ vs EW↑ with Higher Inclination",
        fontsize=11, loc="left",
    )
    ax.legend(loc="upper left")
    ax.axhline(1.5, color="gray", linestyle="--", alpha=0.5, linewidth=1)
    ax.grid(axis="y", alpha=0.3)

    fig.suptitle("Inclination Directional Impact", fontsize=13, fontweight="bold")
    fig.tight_layout()
    fig.savefig(os.path.join(out_dir, "fig_D_inclination_stretch.png"), dpi=200, bbox_inches="tight")
    plt.close(fig)


# ==================== 图E ====================
def plot_E_architecture_capacity(df: pd.DataFrame, out_dir: str) -> None:
    """架构-吞吐与可达流量：单层vs多层（对应PPT第8页）"""
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5.2))

    labels = df["Architecture"].tolist()
    x_pos = np.arange(len(labels))

    # 左：Throughput
    bars1 = ax1.bar(x_pos, df["Throughput_Gbps"], color=["#1f77b4", "#ff7f0e"], alpha=0.9, width=0.45)
    ax1.set_xticks(x_pos)
    ax1.set_xticklabels(labels, rotation=12, ha="right", fontsize=10)
    ax1.set_ylabel("Throughput (Gbps)", fontsize=12)
    ax1.set_title("Total Throughput", fontsize=12)
    ax1.grid(axis="y", alpha=0.3)
    for bar, val in zip(bars1, df["Throughput_Gbps"]):
        ax1.text(
            bar.get_x() + bar.get_width() / 2, bar.get_height() + 40,
            f"{val:.0f}", ha="center", va="bottom", fontsize=10, fontweight="bold",
        )

    # 右：Accommodated Flow
    bars2 = ax2.bar(x_pos, df["Total_Accommodated_Flow_%"], color=["#1f77b4", "#ff7f0e"], alpha=0.9, width=0.45)
    ax2.set_xticks(x_pos)
    ax2.set_xticklabels(labels, rotation=12, ha="right", fontsize=10)
    ax2.set_ylabel("Accommodated Flow (%)", fontsize=12)
    ax2.set_title("Accommodated Flow Ratio", fontsize=12)
    ax2.grid(axis="y", alpha=0.3)
    for bar, val in zip(bars2, df["Total_Accommodated_Flow_%"]):
        ax2.text(
            bar.get_x() + bar.get_width() / 2, bar.get_height() + 0.4,
            f"{val:.1f}%", ha="center", va="bottom", fontsize=10, fontweight="bold",
        )

    fig.suptitle(
        "Architecture Trade-off: Single-Shell Dense vs Multi-Shell Separated",
        fontsize=13, fontweight="bold",
    )
    fig.tight_layout()
    fig.savefig(os.path.join(out_dir, "fig_E_architecture_capacity.png"), dpi=200, bbox_inches="tight")
    plt.close(fig)


# ==================== 图F ====================
def plot_F_architecture_quality(df: pd.DataFrame, out_dir: str) -> None:
    """架构-路径质量：Stretch与HopCount细节（对应PPT第9页）"""
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 5.5))

    labels = df["Architecture"].tolist()
    x_pos = np.arange(len(labels))
    width = 0.15

    stretch_cols = ["NS_Stretch", "EW_Stretch", "HG_Stretch", "NESW_Stretch", "LG_Stretch"]
    stretch_names = ["NS", "EW", "HG", "NESW", "LG"]
    colors = ["#1f77b4", "#ff7f0e", "#2ca02c", "#d62728", "#9467bd"]

    # 左：Stretch
    for i, (col, name, color) in enumerate(zip(stretch_cols, stretch_names, colors)):
        offset = (i - 2) * width
        ax1.bar(x_pos + offset, df[col], width, label=name, color=color, alpha=0.85)
    ax1.set_xticks(x_pos)
    ax1.set_xticklabels(labels, rotation=12, ha="right", fontsize=10)
    ax1.set_ylabel("Stretch", fontsize=12)
    ax1.set_title("Stretch Comparison", fontsize=12)
    ax1.legend(loc="upper left", ncol=3, fontsize=9)
    ax1.grid(axis="y", alpha=0.3)

    # 右：Hop Count
    hop_cols = ["NS_HopCount", "EW_HopCount", "HG_HopCount", "NESW_HopCount", "LG_HopCount"]
    hop_names = ["NS", "EW", "HG", "NESW", "LG"]
    for i, (col, name, color) in enumerate(zip(hop_cols, hop_names, colors)):
        offset = (i - 2) * width
        ax2.bar(x_pos + offset, df[col], width, label=name, color=color, alpha=0.85)
    ax2.set_xticks(x_pos)
    ax2.set_xticklabels(labels, rotation=12, ha="right", fontsize=10)
    ax2.set_ylabel("Median Hop Count", fontsize=12)
    ax2.set_title("Hop Count Comparison", fontsize=12)
    ax2.legend(loc="upper left", ncol=3, fontsize=9)
    ax2.grid(axis="y", alpha=0.3)

    fig.suptitle("Architecture Path-Quality Details", fontsize=13, fontweight="bold")
    fig.tight_layout()
    fig.savefig(os.path.join(out_dir, "fig_F_architecture_quality.png"), dpi=200, bbox_inches="tight")
    plt.close(fig)


# ==================== 图G ====================
def plot_G_temporal_stability(df: pd.DataFrame, out_dir: str) -> None:
    """24h时变稳定性：吞吐波动与NS Stretch（对应PPT第10页）"""
    df = df.sort_values("Time_Hour")
    t = df["Time_Hour"]

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(11, 7.2), sharex=True)

    # 上：Throughput
    ax1.plot(t, df["Throughput_Gbps"], color="#1f77b4", marker="o", linewidth=2, markersize=5, label="Throughput", zorder=3)
    mean_th = df["Throughput_Gbps"].mean()
    min_th = df["Throughput_Gbps"].min()
    max_th = df["Throughput_Gbps"].max()
    ax1.axhline(mean_th, color="gray", linestyle="--", linewidth=1.5, alpha=0.7, label=f"Mean={mean_th:.0f}")
    ax1.fill_between(t, min_th, max_th, color="#1f77b4", alpha=0.08)
    ax1.set_ylabel("Throughput (Gbps)", fontsize=12)
    variation = (max_th - min_th) / mean_th * 100 / 2
    ax1.set_title(
        f"Throughput Variation: {min_th:.0f}–{max_th:.0f} Gbps (±{variation:.1f}%)",
        fontsize=11, loc="left",
    )
    ax1.legend(loc="upper right")
    ax1.grid(axis="y", alpha=0.3)

    # 下：NS Stretch
    ax2.plot(t, df["NS_Stretch"], color="#d62728", marker="s", linewidth=2, markersize=5, label="NS Stretch", zorder=3)
    ax2.set_xlabel("Time (hour)", fontsize=12)
    ax2.set_ylabel("NS Stretch", fontsize=12)
    ax2.set_title("NS Route Stretch Fluctuation", fontsize=11, loc="left")
    ax2.legend(loc="upper right")
    ax2.grid(axis="y", alpha=0.3)

    fig.suptitle("24-Hour Temporal Stability", fontsize=13, fontweight="bold")
    fig.tight_layout()
    fig.savefig(os.path.join(out_dir, "fig_G_temporal_stability.png"), dpi=200, bbox_inches="tight")
    plt.close(fig)


# ==================== 图H ====================
def plot_H_tm_ranking(df: pd.DataFrame, out_dir: str) -> None:
    """TM吞吐排序：Country Capitals最高（对应PPT第11页）"""
    order = ["High_Population_TM", "High_GDP_Population_TM", "Country_Capitals_TM"]
    df_ordered = df.set_index("Traffic_Matrix").reindex(order).reset_index()

    labels = ["High-Pop", "High-GDP", "Country-Cap"]
    x_pos = np.arange(len(labels))

    fig, ax = plt.subplots(figsize=(9, 5.5))
    colors = ["#aec7e8", "#ffbb78", "#98df8a"]
    bars = ax.bar(
        x_pos, df_ordered["Throughput_Gbps"], color=colors, alpha=0.95,
        width=0.5, edgecolor="black", linewidth=0.6,
    )

    # 数值标注
    for bar, val in zip(bars, df_ordered["Throughput_Gbps"]):
        ax.text(
            bar.get_x() + bar.get_width() / 2, bar.get_height() + 60,
            f"{val:.0f}", ha="center", va="bottom", fontsize=11, fontweight="bold",
        )

    # 提升百分比标注（相对High-Pop）
    base = df_ordered.iloc[0]["Throughput_Gbps"]
    for i in range(1, len(df_ordered)):
        val = df_ordered.iloc[i]["Throughput_Gbps"]
        pct = (val - base) / base * 100
        ax.annotate(
            f"+{pct:.1f}%", xy=(x_pos[i], val / 2),
            ha="center", va="center", fontsize=10, color="black", fontweight="bold",
        )

    ax.set_xticks(x_pos)
    ax.set_xticklabels(labels, fontsize=11)
    ax.set_ylabel("Throughput (Gbps)", fontsize=12)
    ax.set_title("Country Capitals TM Achieves Highest Throughput", fontsize=11, loc="left")
    ax.set_ylim(0, df_ordered["Throughput_Gbps"].max() * 1.18)
    ax.grid(axis="y", alpha=0.3)

    fig.suptitle("Traffic Matrix Validation (Consistent with Appendix A.2)", fontsize=13, fontweight="bold")
    fig.tight_layout()
    fig.savefig(os.path.join(out_dir, "fig_H_tm_ranking.png"), dpi=200, bbox_inches="tight")
    plt.close(fig)


# ==================== 图I ====================
def plot_I_congestion_profile(df: pd.DataFrame, out_dir: str) -> None:
    """拥塞与链路活跃度验证（对应PPT第12页）"""
    order = ["High_Population_TM", "High_GDP_Population_TM", "Country_Capitals_TM"]
    df = df.set_index("Traffic_Matrix").reindex(order).reset_index()
    labels = ["High-Pop", "High-GDP", "Country-Cap"]
    x_pos = np.arange(len(labels))

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8.5))

    # 上：Active ISLs Count
    bars = ax1.bar(x_pos, df["Active_ISLs_Count"], color="#2ca02c", alpha=0.85, width=0.4)
    ax1.set_xticks(x_pos)
    ax1.set_xticklabels(labels, fontsize=11)
    ax1.set_ylabel("Active ISLs Count", fontsize=12)
    ax1.set_title(
        "Country Capitals Uses More Active ISLs (Load Spread Across Links)",
        fontsize=11, loc="left",
    )
    for bar, val in zip(bars, df["Active_ISLs_Count"]):
        ax1.text(
            bar.get_x() + bar.get_width() / 2, bar.get_height() + 60,
            f"{val:.0f}", ha="center", va="bottom", fontsize=10, fontweight="bold",
        )
    ax1.grid(axis="y", alpha=0.3)

    # 下：堆叠百分比
    bottom = np.zeros(len(labels))
    colors = ["#98df8a", "#ffbb78", "#aec7e8", "#ff9896"]
    names = ["Low (<20%)", "Medium (20–60%)", "High (60–80%)", "Congested (≥80%)"]
    cols = ["Low_All_%", "Medium_All_%", "High_All_%", "Congested_All_%"]

    for col, name, color in zip(cols, names, colors):
        vals = df[col].values
        ax2.bar(x_pos, vals, bottom=bottom, label=name, color=color, width=0.4, edgecolor="white", linewidth=0.5)
        bottom += vals

    ax2.set_xticks(x_pos)
    ax2.set_xticklabels(labels, fontsize=11)
    ax2.set_ylabel("Utilization Ratio (%)", fontsize=12)
    ax2.set_title("ISL Utilization Breakdown (All ISLs)", fontsize=11, loc="left")
    ax2.legend(loc="upper right", fontsize=9)
    ax2.set_ylim(0, 100)
    ax2.grid(axis="y", alpha=0.3)

    fig.suptitle("Congestion & Link Activity Validation", fontsize=13, fontweight="bold")
    fig.tight_layout()
    fig.savefig(os.path.join(out_dir, "fig_I_congestion_profile.png"), dpi=200, bbox_inches="tight")
    plt.close(fig)


# ==================== Main ====================
def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="生成9张PPT专用可视化图，基于mytests/下的CSV数据"
    )
    parser.add_argument(
        "--mytests-dir", default="./mytests",
        help="CSV数据目录 (默认: ./mytests)",
    )
    parser.add_argument(
        "--output-dir", default="./mytests/figures2",
        help="图片输出目录 (默认: ./mytests/figures2)",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    ensure_dir(args.output_dir)

    # 加载数据
    alt = load_csv_or_raise(os.path.join(args.mytests_dir, "altitude_results.csv"))
    inc = load_csv_or_raise(os.path.join(args.mytests_dir, "inclination_results.csv"))
    arch = load_csv_or_raise(os.path.join(args.mytests_dir, "architecture_results.csv"))
    tmp = load_csv_or_raise(os.path.join(args.mytests_dir, "temporal_results.csv"))
    tm = load_csv_or_raise(os.path.join(args.mytests_dir, "traffic_matrix_results.csv"))
    cong = load_csv_or_raise(os.path.join(args.mytests_dir, "congestion_results.csv"))

    # 绘图
    plot_A_altitude_throughput(alt, args.output_dir)
    plot_B_altitude_path_quality(alt, args.output_dir)
    plot_C_inclination_throughput(inc, args.output_dir)
    plot_D_inclination_stretch(inc, args.output_dir)
    plot_E_architecture_capacity(arch, args.output_dir)
    plot_F_architecture_quality(arch, args.output_dir)
    plot_G_temporal_stability(tmp, args.output_dir)
    plot_H_tm_ranking(tm, args.output_dir)
    plot_I_congestion_profile(cong, args.output_dir)

    print(f"Done. 9 presentation figures saved to: {args.output_dir}")


if __name__ == "__main__":
    main()

"""使用 matplotlib 对 mytests 结果做多指标对比图。

设计原则：
1. 每个实验按“论文关注点”做多子图，而非只看吞吐量。
2. 图中同时展示 throughput / accommodated flow / stretch / hop / route selection。
3. 对关键论文结论做程序化校验，避免画出与结论相反的图。
"""

from __future__ import annotations

import argparse
import os
import sys

if __package__ in (None, ""):
    sys.path.append(os.path.dirname(os.path.dirname(__file__)))

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


plt.style.use("seaborn-v0_8-whitegrid")


def load_csv_or_raise(path: str) -> pd.DataFrame:
    if not os.path.exists(path):
        raise FileNotFoundError(f"CSV 不存在: {path}")
    return pd.read_csv(path)


def ensure_dir(path: str) -> None:
    os.makedirs(path, exist_ok=True)


def _line(ax, x, y, label, color, marker="o", lw=2.0):
    ax.plot(x, y, label=label, color=color, marker=marker, linewidth=lw, markersize=5)


def assert_tm_conclusion(tm_df: pd.DataFrame) -> None:
    ranking = tm_df.sort_values("Throughput_Gbps", ascending=False)
    top_tm = str(ranking.iloc[0]["Traffic_Matrix"])
    if top_tm != "Country_Capitals_TM":
        raise ValueError(
            "与论文附录 A.2 结论不一致：当前 TM 吞吐最高不是 Country_Capitals_TM。"
            f" 实际最高为: {top_tm}"
        )


def plot_altitude_dashboard(df: pd.DataFrame, out_dir: str) -> None:
    """对应论文 Fig.20（随高度变化的吞吐/stretch/hop）。"""
    df = df.sort_values("Altitude_km")
    x = df["Altitude_km"]

    fig, axs = plt.subplots(2, 2, figsize=(14, 9))
    fig.suptitle("Altitude Experiment Dashboard (Fig.20-aligned)", fontsize=14, fontweight="bold")

    # 1) Throughput + Accommodated flow
    ax = axs[0, 0]
    _line(ax, x, df["Throughput_Gbps"], "Throughput (Gbps)", "#1f77b4")
    ax2 = ax.twinx()
    _line(ax2, x, df["Total_Accommodated_Flow_%"], "Accommodated Flow (%)", "#d62728", marker="s")
    ax.set_title("Throughput and Accommodated Flow vs Altitude")
    ax.set_xlabel("Altitude (km)")
    ax.set_ylabel("Throughput (Gbps)")
    ax2.set_ylabel("Accommodated Flow (%)")
    l1, lb1 = ax.get_legend_handles_labels()
    l2, lb2 = ax2.get_legend_handles_labels()
    ax.legend(l1 + l2, lb1 + lb2, loc="best")

    # 2) Stretch by route category
    ax = axs[0, 1]
    _line(ax, x, df["NS_Stretch"], "NS", "#2ca02c")
    _line(ax, x, df["EW_Stretch"], "EW", "#9467bd")
    _line(ax, x, df["HG_Stretch"], "HG", "#8c564b")
    _line(ax, x, df["LG_Stretch"], "LG", "#ff7f0e")
    ax.set_title("Stretch vs Altitude")
    ax.set_xlabel("Altitude (km)")
    ax.set_ylabel("Stretch")
    ax.legend(loc="best", ncol=2)

    # 3) Hop counts by route category
    ax = axs[1, 0]
    _line(ax, x, df["NS_HopCount"], "NS Hop", "#17becf")
    _line(ax, x, df["EW_HopCount"], "EW Hop", "#bcbd22")
    _line(ax, x, df["HG_HopCount"], "HG Hop", "#7f7f7f")
    _line(ax, x, df["LG_HopCount"], "LG Hop", "#e377c2")
    ax.set_title("Hop Count vs Altitude")
    ax.set_xlabel("Altitude (km)")
    ax.set_ylabel("Median Hop Count")
    ax.legend(loc="best", ncol=2)

    # 4) Route selection proportions
    ax = axs[1, 1]
    _line(ax, x, df["NS_Selected_%"], "NS Selected %", "#1f77b4")
    _line(ax, x, df["EW_Selected_%"], "EW Selected %", "#ff7f0e")
    _line(ax, x, df["NESW_Selected_%"], "NESW Selected %", "#2ca02c")
    _line(ax, x, df["HG_Selected_%"], "HG Selected %", "#d62728")
    _line(ax, x, df["LG_Selected_%"], "LG Selected %", "#9467bd")
    ax.set_title("Path Selection vs Altitude")
    ax.set_xlabel("Altitude (km)")
    ax.set_ylabel("Selection Ratio (%)")
    ax.legend(loc="best", ncol=2)

    fig.tight_layout(rect=[0, 0, 1, 0.97])
    fig.savefig(os.path.join(out_dir, "dashboard_altitude.png"), dpi=200)
    plt.close(fig)


def plot_inclination_dashboard(df: pd.DataFrame, out_dir: str) -> None:
    """对应论文 Fig.21（低倾角会拉高部分 stretch，吞吐与覆盖有权衡）。"""
    df = df.sort_values("Inclination_Degree")
    x = df["Inclination_Degree"]

    fig, axs = plt.subplots(2, 2, figsize=(14, 9))
    fig.suptitle("Inclination Experiment Dashboard (Fig.21-aligned)", fontsize=14, fontweight="bold")

    # 1) Throughput + Accommodated flow
    ax = axs[0, 0]
    _line(ax, x, df["Throughput_Gbps"], "Throughput (Gbps)", "#1f77b4")
    ax2 = ax.twinx()
    _line(ax2, x, df["Total_Accommodated_Flow_%"], "Accommodated Flow (%)", "#d62728", marker="s")
    ax.axvline(53.0, color="gray", linestyle=":", alpha=0.8)
    ax.set_title("Throughput and Accommodated Flow vs Inclination")
    ax.set_xlabel("Inclination (degree)")
    ax.set_ylabel("Throughput (Gbps)")
    ax2.set_ylabel("Accommodated Flow (%)")
    l1, lb1 = ax.get_legend_handles_labels()
    l2, lb2 = ax2.get_legend_handles_labels()
    ax.legend(l1 + l2, lb1 + lb2, loc="best")

    # 2) Stretch focus (paper explicitly discusses NS/LG inflation at lower inclination)
    ax = axs[0, 1]
    _line(ax, x, df["NS_Stretch"], "NS Stretch", "#2ca02c")
    _line(ax, x, df["LG_Stretch"], "LG Stretch", "#ff7f0e")
    _line(ax, x, df["EW_Stretch"], "EW Stretch", "#9467bd")
    _line(ax, x, df["HG_Stretch"], "HG Stretch", "#8c564b")
    ax.axvspan(x.min(), 40, color="#f2f2f2", alpha=0.7, label="Low inclination zone")
    ax.set_title("Stretch vs Inclination")
    ax.set_xlabel("Inclination (degree)")
    ax.set_ylabel("Stretch")
    ax.legend(loc="best", ncol=2)

    # 3) Hop counts
    ax = axs[1, 0]
    _line(ax, x, df["NS_HopCount"], "NS Hop", "#17becf")
    _line(ax, x, df["EW_HopCount"], "EW Hop", "#bcbd22")
    _line(ax, x, df["HG_HopCount"], "HG Hop", "#7f7f7f")
    _line(ax, x, df["LG_HopCount"], "LG Hop", "#e377c2")
    ax.set_title("Hop Count vs Inclination")
    ax.set_xlabel("Inclination (degree)")
    ax.set_ylabel("Median Hop Count")
    ax.legend(loc="best", ncol=2)

    # 4) Route selection proportions
    ax = axs[1, 1]
    _line(ax, x, df["NS_Selected_%"], "NS Selected %", "#1f77b4")
    _line(ax, x, df["EW_Selected_%"], "EW Selected %", "#ff7f0e")
    _line(ax, x, df["NESW_Selected_%"], "NESW Selected %", "#2ca02c")
    _line(ax, x, df["HG_Selected_%"], "HG Selected %", "#d62728")
    _line(ax, x, df["LG_Selected_%"], "LG Selected %", "#9467bd")
    ax.set_title("Path Selection vs Inclination")
    ax.set_xlabel("Inclination (degree)")
    ax.set_ylabel("Selection Ratio (%)")
    ax.legend(loc="best", ncol=2)

    fig.tight_layout(rect=[0, 0, 1, 0.97])
    fig.savefig(os.path.join(out_dir, "dashboard_inclination.png"), dpi=200)
    plt.close(fig)


def plot_architecture_dashboard(df: pd.DataFrame, out_dir: str) -> None:
    """对应论文中单层/多层设计对比思路：吞吐与延迟类指标权衡。"""
    d = df.copy().reset_index(drop=True)
    labels = d["Architecture"].tolist()
    x = np.arange(len(labels))

    fig, axs = plt.subplots(1, 3, figsize=(16, 5.2))
    fig.suptitle("Architecture Dashboard (Single vs Multi Shell)", fontsize=14, fontweight="bold")

    # 1) Throughput + accommodated
    ax = axs[0]
    w = 0.35
    ax.bar(x - w / 2, d["Throughput_Gbps"], width=w, color="#1f77b4", label="Throughput (Gbps)")
    ax2 = ax.twinx()
    ax2.bar(x + w / 2, d["Total_Accommodated_Flow_%"], width=w, color="#ff7f0e", label="Accommodated Flow (%)")
    ax.set_xticks(x)
    ax.set_xticklabels(labels, rotation=10)
    ax.set_title("Capacity")
    ax.set_ylabel("Throughput (Gbps)")
    ax2.set_ylabel("Accommodated Flow (%)")
    l1, lb1 = ax.get_legend_handles_labels()
    l2, lb2 = ax2.get_legend_handles_labels()
    ax.legend(l1 + l2, lb1 + lb2, loc="upper left", fontsize=8)

    # 2) Stretch grouped bars
    ax = axs[1]
    cats = ["NS_Stretch", "EW_Stretch", "HG_Stretch", "LG_Stretch", "NESW_Stretch"]
    pos = np.arange(len(cats))
    w = 0.35
    ax.bar(pos - w / 2, d.loc[0, cats], width=w, color="#9467bd", label=labels[0])
    ax.bar(pos + w / 2, d.loc[1, cats], width=w, color="#2ca02c", label=labels[1])
    ax.set_xticks(pos)
    ax.set_xticklabels([c.replace("_Stretch", "") for c in cats])
    ax.set_title("Stretch Comparison")
    ax.set_ylabel("Stretch")
    ax.legend(fontsize=8)

    # 3) Hopcount grouped bars
    ax = axs[2]
    cats = ["NS_HopCount", "EW_HopCount", "HG_HopCount", "LG_HopCount", "NESW_HopCount"]
    pos = np.arange(len(cats))
    w = 0.35
    ax.bar(pos - w / 2, d.loc[0, cats], width=w, color="#9467bd", label=labels[0])
    ax.bar(pos + w / 2, d.loc[1, cats], width=w, color="#2ca02c", label=labels[1])
    ax.set_xticks(pos)
    ax.set_xticklabels([c.replace("_HopCount", "") for c in cats])
    ax.set_title("Hop Count Comparison")
    ax.set_ylabel("Median Hop Count")
    ax.legend(fontsize=8)

    fig.tight_layout(rect=[0, 0, 1, 0.95])
    fig.savefig(os.path.join(out_dir, "dashboard_architecture.png"), dpi=200)
    plt.close(fig)


def plot_temporal_dashboard(df: pd.DataFrame, out_dir: str) -> None:
    """对应论文中的时变路由现象：指标有波动但整体规律稳定。"""
    d = df.sort_values("Time_Hour")
    t = d["Time_Hour"]

    fig, axs = plt.subplots(2, 2, figsize=(14, 9))
    fig.suptitle("Temporal Evolution Dashboard (24h)", fontsize=14, fontweight="bold")

    # 1) Throughput over time
    ax = axs[0, 0]
    _line(ax, t, d["Throughput_Gbps"], "Throughput", "#1f77b4")
    mean_th = d["Throughput_Gbps"].mean()
    ax.axhline(mean_th, color="gray", linestyle="--", label=f"Mean={mean_th:.1f}")
    ax.set_title("Throughput over Time")
    ax.set_xlabel("Hour")
    ax.set_ylabel("Gbps")
    ax.legend(loc="best")

    # 2) Accommodated flow + route selection
    ax = axs[0, 1]
    _line(ax, t, d["Total_Accommodated_Flow_%"], "Accommodated Flow %", "#d62728")
    _line(ax, t, d["NS_Selected_%"], "NS Selected %", "#2ca02c", marker="s")
    _line(ax, t, d["EW_Selected_%"], "EW Selected %", "#9467bd", marker="^")
    ax.set_title("Flow Accommodation and Path Selection")
    ax.set_xlabel("Hour")
    ax.set_ylabel("Percent")
    ax.legend(loc="best")

    # 3) Stretch stability
    ax = axs[1, 0]
    _line(ax, t, d["NS_Stretch"], "NS Stretch", "#17becf")
    _line(ax, t, d["EW_Stretch"], "EW Stretch", "#ff7f0e")
    _line(ax, t, d["HG_Stretch"], "HG Stretch", "#8c564b")
    ax.set_title("Stretch over Time")
    ax.set_xlabel("Hour")
    ax.set_ylabel("Stretch")
    ax.legend(loc="best")

    # 4) Hop count stability
    ax = axs[1, 1]
    _line(ax, t, d["NS_HopCount"], "NS Hop", "#1f77b4")
    _line(ax, t, d["EW_HopCount"], "EW Hop", "#ff9896")
    _line(ax, t, d["HG_HopCount"], "HG Hop", "#7f7f7f")
    ax.set_title("Hop Count over Time")
    ax.set_xlabel("Hour")
    ax.set_ylabel("Median Hop Count")
    ax.legend(loc="best")

    fig.tight_layout(rect=[0, 0, 1, 0.97])
    fig.savefig(os.path.join(out_dir, "dashboard_temporal.png"), dpi=200)
    plt.close(fig)


def plot_tm_dashboard(tm_df: pd.DataFrame, congestion_df: pd.DataFrame, out_dir: str) -> None:
    """对应论文 Fig.24/A.2：Country capital TM 吞吐更高，且热点拥塞不更严重。"""
    assert_tm_conclusion(tm_df)

    order = ["High_Population_TM", "High_GDP_Population_TM", "Country_Capitals_TM"]
    tmd = tm_df.set_index("Traffic_Matrix").loc[order].reset_index()
    cong = congestion_df.set_index("Traffic_Matrix").loc[order].reset_index()

    fig, axs = plt.subplots(2, 2, figsize=(14, 9))
    fig.suptitle("Traffic-Matrix Dashboard (Fig.24/A.2-aligned)", fontsize=14, fontweight="bold")

    x = np.arange(len(order))

    # 1) Throughput + accommodated
    ax = axs[0, 0]
    w = 0.35
    ax.bar(x - w / 2, tmd["Throughput_Gbps"], width=w, color="#1f77b4", label="Throughput (Gbps)")
    ax2 = ax.twinx()
    ax2.bar(x + w / 2, tmd["Total_Accommodated_Flow_%"], width=w, color="#ff7f0e", label="Accommodated Flow (%)")
    ax.set_xticks(x)
    ax.set_xticklabels(order, rotation=10)
    ax.set_title("TM Capacity Comparison")
    ax.set_ylabel("Throughput (Gbps)")
    ax2.set_ylabel("Accommodated Flow (%)")
    l1, lb1 = ax.get_legend_handles_labels()
    l2, lb2 = ax2.get_legend_handles_labels()
    ax.legend(l1 + l2, lb1 + lb2, loc="upper left", fontsize=8)

    # 2) Stretch and hop tradeoff
    ax = axs[0, 1]
    _line(ax, order, tmd["NS_Stretch"], "NS Stretch", "#2ca02c")
    _line(ax, order, tmd["EW_Stretch"], "EW Stretch", "#9467bd")
    ax2 = ax.twinx()
    _line(ax2, order, tmd["NS_HopCount"], "NS Hop", "#d62728", marker="s")
    ax.set_title("Stretch/Hop across TMs")
    ax.set_ylabel("Stretch")
    ax2.set_ylabel("Hop Count")
    l1, lb1 = ax.get_legend_handles_labels()
    l2, lb2 = ax2.get_legend_handles_labels()
    ax.legend(l1 + l2, lb1 + lb2, loc="best", fontsize=8)

    # 3) Active-link utilization profile
    ax = axs[1, 0]
    _line(ax, order, cong["Avg_Utilization_Active_%"], "Avg Active Util %", "#17becf")
    _line(ax, order, cong["P95_Utilization_Active_%"], "P95 Active Util %", "#bcbd22")
    _line(ax, order, cong["Max_Utilization_Active_%"], "Max Active Util %", "#e377c2")
    ax.set_title("Active ISL Utilization Profile")
    ax.set_ylabel("Utilization (%)")
    ax.legend(loc="best")

    # 4) Active utilization composition
    ax = axs[1, 1]
    low = cong["Low_Active_%"].values
    med = cong["Medium_Active_%"].values
    high = cong["High_Active_%"].values
    con = cong["Congested_Active_%"].values
    ax.bar(order, low, label="Low (<20%)", color="#98df8a")
    ax.bar(order, med, bottom=low, label="Medium (20-60%)", color="#ffbb78")
    ax.bar(order, high, bottom=low + med, label="High (60-80%)", color="#aec7e8")
    ax.bar(order, con, bottom=low + med + high, label="Congested (>=80%)", color="#ff9896")
    ax.set_ylim(0, 100)
    ax.set_title("Active ISL Utilization Breakdown")
    ax.set_ylabel("Ratio (%)")
    ax.legend(loc="upper right", fontsize=8)

    fig.tight_layout(rect=[0, 0, 1, 0.97])
    fig.savefig(os.path.join(out_dir, "dashboard_tm_congestion.png"), dpi=200)
    plt.close(fig)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="使用 matplotlib 绘制 mytests 多指标数据对比图")
    parser.add_argument("--mytests-dir", default="./mytests", help="mytests 目录")
    parser.add_argument("--output-dir", default="./mytests/figures", help="图片输出目录")
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    ensure_dir(args.output_dir)

    altitude_df = load_csv_or_raise(os.path.join(args.mytests_dir, "altitude_results.csv"))
    inclination_df = load_csv_or_raise(os.path.join(args.mytests_dir, "inclination_results.csv"))
    architecture_df = load_csv_or_raise(os.path.join(args.mytests_dir, "architecture_results.csv"))
    temporal_df = load_csv_or_raise(os.path.join(args.mytests_dir, "temporal_results.csv"))
    tm_df = load_csv_or_raise(os.path.join(args.mytests_dir, "traffic_matrix_results.csv"))
    congestion_df = load_csv_or_raise(os.path.join(args.mytests_dir, "congestion_results.csv"))

    plot_altitude_dashboard(altitude_df, args.output_dir)
    plot_inclination_dashboard(inclination_df, args.output_dir)
    plot_architecture_dashboard(architecture_df, args.output_dir)
    plot_temporal_dashboard(temporal_df, args.output_dir)
    plot_tm_dashboard(tm_df, congestion_df, args.output_dir)

    print(f"Done. Rich dashboards saved to: {args.output_dir}")


if __name__ == "__main__":
    main()

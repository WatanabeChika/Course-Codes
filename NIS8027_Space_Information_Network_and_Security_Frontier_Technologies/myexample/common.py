"""myexample 的公共实验工具。

目标：
1. 统一基线参数，避免脚本间口径漂移。
2. 复用星座构建、性能计算、CSV 导出逻辑。
3. 支持快速抽样运行（由各脚本通过 --limit 控制）。
"""

from __future__ import annotations

import csv
import os
from dataclasses import dataclass

from LEOCraft.attenuation.fspl import FSPL
from LEOCraft.constellations.LEO_constellation import LEOConstellation
from LEOCraft.performance.basic.stretch import Stretch
from LEOCraft.performance.basic.throughput import Throughput
from LEOCraft.satellite_topology.plus_grid_shell import PlusGridShell
from LEOCraft.user_terminals.ground_station import GroundStation

# 基线链路参数（与 README / 示例保持一致）
BASE_FREQUENCY_HZ = 28.5 * 1_000_000_000
BASE_TX_POWER_DBM = 98.4
BASE_BANDWIDTH_HZ = 0.5 * 1_000_000_000
BASE_GT_RATIO = 13.6
BASE_TX_ANTENNA_GAIN_DB = 34.5

# 基线壳层参数（用于单层实验）
BASE_ORBITS = 72
BASE_SATS_PER_ORBIT = 22
BASE_ALTITUDE_M = 550_000.0
BASE_INCLINATION_DEG = 53.0
BASE_ELEVATION_DEG = 25.0
BASE_PHASE_OFFSET = 50.0


@dataclass(frozen=True)
class ShellConfig:
    """单个 shell 的实验参数。"""

    shell_id: int
    orbits: int
    sat_per_orbit: int
    altitude_m: float
    inclination_degree: float
    angle_of_elevation_degree: float = BASE_ELEVATION_DEG
    phase_offset: float = BASE_PHASE_OFFSET


def build_baseline_loss_model() -> FSPL:
    """构建基线 FSPL 模型。"""
    loss_model = FSPL(
        BASE_FREQUENCY_HZ,
        BASE_TX_POWER_DBM,
        BASE_BANDWIDTH_HZ,
        BASE_GT_RATIO,
    )
    loss_model.set_Tx_antenna_gain(gain_dB=BASE_TX_ANTENNA_GAIN_DB)
    return loss_model


def ensure_parent_dir(path: str) -> None:
    """确保输出文件的父目录存在。"""
    parent = os.path.dirname(path)
    if parent:
        os.makedirs(parent, exist_ok=True)


def total_satellites(shells: list[ShellConfig]) -> int:
    """统计配置中的卫星总数。"""
    return sum(shell.orbits * shell.sat_per_orbit for shell in shells)


def build_and_route_constellation(
    name: str,
    ground_station_dataset: str,
    shells: list[ShellConfig],
    time_elapsed_seconds: int = 0,
    verbose: bool = False,
) -> LEOConstellation:
    """构建星座并完成路由生成。"""
    leo_con = LEOConstellation(name)
    leo_con.v.verbose = verbose

    leo_con.add_ground_stations(GroundStation(ground_station_dataset))

    for shell in shells:
        leo_con.add_shells(
            PlusGridShell(
                id=shell.shell_id,
                orbits=shell.orbits,
                sat_per_orbit=shell.sat_per_orbit,
                altitude_m=shell.altitude_m,
                inclination_degree=shell.inclination_degree,
                angle_of_elevation_degree=shell.angle_of_elevation_degree,
                phase_offset=shell.phase_offset,
            )
        )

    leo_con.set_time(second=time_elapsed_seconds)
    leo_con.set_loss_model(build_baseline_loss_model())
    leo_con.build()
    leo_con.create_network_graph()
    leo_con.generate_routes()
    return leo_con


def compute_throughput_and_stretch(
    leo_con: LEOConstellation,
    tm_dataset: str,
) -> dict[str, float]:
    """计算吞吐与 stretch/hop 指标并返回统一字典。"""
    throughput = Throughput(leo_con, tm_dataset)
    throughput.build()
    throughput.compute()

    stretch = Stretch(leo_con)
    stretch.build()
    stretch.compute()

    return metrics_from_models(throughput, stretch)


def metrics_from_models(throughput: Throughput, stretch: Stretch) -> dict[str, float]:
    """将 throughput/stretch 实例统一映射为汇总指标字典。"""
    return {
        "Throughput_Gbps": round(throughput.throughput_Gbps, 2),
        "Total_Accommodated_Flow_%": round(throughput.total_accommodated_flow, 3),
        "NS_Selected_%": round(throughput.NS_selt, 3),
        "EW_Selected_%": round(throughput.EW_selt, 3),
        "NESW_Selected_%": round(throughput.NESW_selt, 3),
        "HG_Selected_%": round(throughput.HG_selt, 3),
        "LG_Selected_%": round(throughput.LG_selt, 3),
        "NS_Stretch": round(stretch.NS_sth, 3),
        "EW_Stretch": round(stretch.EW_sth, 3),
        "HG_Stretch": round(stretch.HG_sth, 3),
        "LG_Stretch": round(stretch.LG_sth, 3),
        "NESW_Stretch": round(stretch.NESW_sth, 3),
        "NS_HopCount": stretch.NS_cnt,
        "EW_HopCount": stretch.EW_cnt,
        "HG_HopCount": stretch.HG_cnt,
        "LG_HopCount": stretch.LG_cnt,
        "NESW_HopCount": stretch.NESW_cnt,
    }


def run_throughput_and_stretch(
    leo_con: LEOConstellation,
    tm_dataset: str,
) -> tuple[Throughput, Stretch]:
    """运行 throughput 与 stretch 并返回实例，便于调用方导出详细数据。"""
    throughput = Throughput(leo_con, tm_dataset)
    throughput.build()
    throughput.compute()

    stretch = Stretch(leo_con)
    stretch.build()
    stretch.compute()
    return throughput, stretch


def write_csv_rows(csv_path: str, rows: list[dict]) -> None:
    """将结果写入 CSV（自动创建目录）。"""
    if not rows:
        return

    ensure_parent_dir(csv_path)
    with open(csv_path, mode="w", newline="", encoding="utf-8") as csv_file:
        writer = csv.DictWriter(csv_file, fieldnames=list(rows[0].keys()))
        writer.writeheader()
        writer.writerows(rows)

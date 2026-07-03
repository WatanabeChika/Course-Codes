"""基础场景：Starlink 单层基线。

用途：
1. 快速验证环境是否正常。
2. 产出完整导出数据供后续分析。
"""

from __future__ import annotations

import argparse
import os
import sys

if __package__ in (None, ""):
    sys.path.append(os.path.dirname(os.path.dirname(__file__)))
import time

from LEOCraft.dataset import GroundStationAtCities, InternetTrafficAcrossCities

from myexample.common import (
    BASE_ALTITUDE_M,
    BASE_INCLINATION_DEG,
    BASE_ORBITS,
    BASE_SATS_PER_ORBIT,
    ShellConfig,
    build_and_route_constellation,
    metrics_from_models,
    run_throughput_and_stretch,
)

DEFAULT_OUTPUT_PATH = "./Starlink"


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="运行 LEOCraft 基础单层场景")
    parser.add_argument("--output-dir", default=DEFAULT_OUTPUT_PATH, help="导出目录")
    parser.add_argument("--verbose", action="store_true", help="打印 LEOCraft 详细日志")
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    start_time = time.perf_counter()

    shells = [
        ShellConfig(
            shell_id=0,
            orbits=BASE_ORBITS,
            sat_per_orbit=BASE_SATS_PER_ORBIT,
            altitude_m=BASE_ALTITUDE_M,
            inclination_degree=BASE_INCLINATION_DEG,
        )
    ]

    leo_con = build_and_route_constellation(
        name="Starlink",
        ground_station_dataset=GroundStationAtCities.TOP_100,
        shells=shells,
        verbose=args.verbose,
    )

    throughput, stretch = run_throughput_and_stretch(
        leo_con,
        InternetTrafficAcrossCities.ONLY_POP_100,
    )
    metrics = metrics_from_models(throughput, stretch)

    # Constellation
    leo_con.export_gsls(args.output_dir)
    leo_con.export_routes(args.output_dir)
    leo_con.export_no_path_found(args.output_dir)
    leo_con.export_k_path_not_found(args.output_dir)

    # Shells
    for shell in leo_con.shells:
        shell.export_satellites(args.output_dir)
        shell.export_isls(args.output_dir)

    # Ground stations
    leo_con.ground_stations.export(args.output_dir)

    # Throughput / Stretch
    throughput.export_path_selection(args.output_dir)
    throughput.export_LP_model(args.output_dir)
    stretch.export_stretch_dataset(args.output_dir)
    elapsed_min = round((time.perf_counter() - start_time) / 60, 2)

    print("Baseline metrics:")
    for key, value in metrics.items():
        print(f"  {key}: {value}")
    print(f"Total simulation time: {elapsed_min}m")


if __name__ == "__main__":
    main()

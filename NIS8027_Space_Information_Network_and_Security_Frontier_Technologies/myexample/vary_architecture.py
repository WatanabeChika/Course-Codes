"""架构选型实验：对比单层与多层架构（保持卫星总数一致）。"""

from __future__ import annotations

import argparse
import os
import sys

if __package__ in (None, ""):
    sys.path.append(os.path.dirname(os.path.dirname(__file__)))
import time

from LEOCraft.dataset import GroundStationAtCities, InternetTrafficAcrossCities

from myexample.common import (
    ShellConfig,
    build_and_route_constellation,
    compute_throughput_and_stretch,
    total_satellites,
    write_csv_rows,
)

DEFAULT_SUMMARY_CSV_PATH = "./mytests/architecture_results.csv"

# 两种架构都使用 72*22 = 1584 颗卫星，避免规模差异干扰结论。
ARCHITECTURES_TO_TEST = [
    {
        "name": "Multi-Shell-Separated",
        "shells": [
            ShellConfig(0, 24, 22, 540_000.0, 53.0),
            ShellConfig(1, 24, 22, 550_000.0, 53.2),
            ShellConfig(2, 24, 22, 560_000.0, 70.0),
        ],
    },
    {
        "name": "Single-Shell-Dense",
        "shells": [
            ShellConfig(0, 72, 22, 550_000.0, 53.0),
        ],
    },
]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="对比单层和多层架构性能")
    parser.add_argument("--output-csv", default=DEFAULT_SUMMARY_CSV_PATH, help="结果 CSV 路径")
    parser.add_argument("--limit", type=int, default=None, help="只运行前 N 个架构，用于快速验证")
    parser.add_argument("--verbose", action="store_true", help="打印 LEOCraft 详细日志")
    return parser.parse_args()


def validate_satellite_budget(architectures: list[dict]) -> None:
    totals = [total_satellites(arch["shells"]) for arch in architectures]
    if len(set(totals)) != 1:
        raise ValueError(f"卫星总数不一致，无法公平对比: {totals}")


def main() -> None:
    args = parse_args()
    start_time = time.perf_counter()

    architectures = ARCHITECTURES_TO_TEST[: args.limit] if args.limit else ARCHITECTURES_TO_TEST
    validate_satellite_budget(architectures)

    all_results: list[dict] = []

    for arch in architectures:
        arch_name = arch["name"]
        shells = arch["shells"]
        sat_count = total_satellites(shells)

        print(f"Evaluating architecture={arch_name}, satellites={sat_count}")

        leo_con = build_and_route_constellation(
            name=f"Starlink_{arch_name}",
            ground_station_dataset=GroundStationAtCities.TOP_100,
            shells=shells,
            verbose=args.verbose,
        )

        current_result = {
            "Architecture": arch_name,
            "Total_Satellites": sat_count,
            "Shell_Count": len(shells),
        }
        current_result.update(
            compute_throughput_and_stretch(
                leo_con,
                InternetTrafficAcrossCities.ONLY_POP_100,
            )
        )
        all_results.append(current_result)

    write_csv_rows(args.output_csv, all_results)
    elapsed_min = round((time.perf_counter() - start_time) / 60, 2)

    print(f"Done. Cases={len(all_results)}")
    print(f"Total time taken: {elapsed_min} minutes")
    print(f"Summary data saved to: {args.output_csv}")


if __name__ == "__main__":
    main()

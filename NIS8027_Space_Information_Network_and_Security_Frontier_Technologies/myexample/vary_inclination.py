"""倾角敏感性实验：固定高度与规模，扫描不同轨道倾角。"""

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
    BASE_ORBITS,
    BASE_SATS_PER_ORBIT,
    ShellConfig,
    build_and_route_constellation,
    compute_throughput_and_stretch,
    write_csv_rows,
)

DEFAULT_SUMMARY_CSV_PATH = "./mytests/inclination_results.csv"

# 53° 为 Starlink Gen1 常见基线；低倾角偏赤道，高倾角增强高纬覆盖
INCLINATIONS_TO_TEST_DEG = [30.0, 40.0, 50.0, 53.0, 60.0, 70.0, 80.0]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="扫描倾角并输出吞吐与 stretch 对比")
    parser.add_argument("--output-csv", default=DEFAULT_SUMMARY_CSV_PATH, help="结果 CSV 路径")
    parser.add_argument("--limit", type=int, default=None, help="只运行前 N 个倾角，用于快速验证")
    parser.add_argument("--verbose", action="store_true", help="打印 LEOCraft 详细日志")
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    start_time = time.perf_counter()

    inclinations = INCLINATIONS_TO_TEST_DEG[: args.limit] if args.limit else INCLINATIONS_TO_TEST_DEG
    all_results: list[dict] = []

    for inc in inclinations:
        print(f"Evaluating inclination={inc} degree")

        shells = [
            ShellConfig(
                shell_id=0,
                orbits=BASE_ORBITS,
                sat_per_orbit=BASE_SATS_PER_ORBIT,
                altitude_m=BASE_ALTITUDE_M,
                inclination_degree=inc,
            )
        ]
        leo_con = build_and_route_constellation(
            name=f"Starlink_Inc_{inc}",
            ground_station_dataset=GroundStationAtCities.TOP_100,
            shells=shells,
            verbose=args.verbose,
        )

        current_result = {"Inclination_Degree": inc}
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

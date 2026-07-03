"""高度敏感性实验：在固定拓扑规模下扫描不同轨道高度。"""

from __future__ import annotations

import argparse
import os
import sys

if __package__ in (None, ""):
    sys.path.append(os.path.dirname(os.path.dirname(__file__)))
import time

from LEOCraft.dataset import GroundStationAtCities, InternetTrafficAcrossCities

from myexample.common import (
    BASE_INCLINATION_DEG,
    BASE_ORBITS,
    BASE_SATS_PER_ORBIT,
    ShellConfig,
    build_and_route_constellation,
    compute_throughput_and_stretch,
    write_csv_rows,
)

DEFAULT_SUMMARY_CSV_PATH = "./mytests/altitude_results.csv"

# 选取若干有代表性的 LEO 高度（米）
ALTITUDES_TO_TEST_M = [
    340_000.0,
    550_000.0,
    610_000.0,
    1_015_000.0,
    1_200_000.0,
    1_400_000.0,
    2_000_000.0,
]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="扫描高度并输出吞吐与 stretch 对比")
    parser.add_argument("--output-csv", default=DEFAULT_SUMMARY_CSV_PATH, help="结果 CSV 路径")
    parser.add_argument("--limit", type=int, default=None, help="只运行前 N 个高度，用于快速验证")
    parser.add_argument("--verbose", action="store_true", help="打印 LEOCraft 详细日志")
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    start_time = time.perf_counter()

    altitudes = ALTITUDES_TO_TEST_M[: args.limit] if args.limit else ALTITUDES_TO_TEST_M
    all_results: list[dict] = []

    for altitude_m in altitudes:
        altitude_km = int(altitude_m / 1000)
        print(f"Evaluating altitude={altitude_km} km")

        shells = [
            ShellConfig(
                shell_id=0,
                orbits=BASE_ORBITS,
                sat_per_orbit=BASE_SATS_PER_ORBIT,
                altitude_m=altitude_m,
                inclination_degree=BASE_INCLINATION_DEG,
            )
        ]
        leo_con = build_and_route_constellation(
            name=f"Starlink_Alt_{altitude_km}",
            ground_station_dataset=GroundStationAtCities.TOP_100,
            shells=shells,
            verbose=args.verbose,
        )

        current_result = {"Altitude_km": altitude_km}
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

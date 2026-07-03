"""时空动态实验：观察一天内不同时刻的网络性能变化。"""

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
    compute_throughput_and_stretch,
    write_csv_rows,
)

DEFAULT_SUMMARY_CSV_PATH = "./mytests/temporal_results.csv"


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="扫描不同时间点的网络性能")
    parser.add_argument("--output-csv", default=DEFAULT_SUMMARY_CSV_PATH, help="结果 CSV 路径")
    parser.add_argument("--step-hours", type=int, default=2, help="时间步长（小时），默认 2")
    parser.add_argument("--end-hour", type=int, default=24, help="终止小时（含），默认 24")
    parser.add_argument("--limit", type=int, default=None, help="只运行前 N 个时间点，用于快速验证")
    parser.add_argument("--verbose", action="store_true", help="打印 LEOCraft 详细日志")
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    start_time = time.perf_counter()

    if args.step_hours <= 0:
        raise ValueError("--step-hours 必须大于 0")
    if args.end_hour < 0:
        raise ValueError("--end-hour 必须大于等于 0")

    time_points_hours = list(range(0, args.end_hour + 1, args.step_hours))
    if args.limit:
        time_points_hours = time_points_hours[: args.limit]

    shells = [
        ShellConfig(
            shell_id=0,
            orbits=BASE_ORBITS,
            sat_per_orbit=BASE_SATS_PER_ORBIT,
            altitude_m=BASE_ALTITUDE_M,
            inclination_degree=BASE_INCLINATION_DEG,
        )
    ]

    all_results: list[dict] = []

    for hour in time_points_hours:
        elapsed_seconds = int(hour * 3600)
        print(f"Evaluating t=+{hour}h ({elapsed_seconds}s)")

        leo_con = build_and_route_constellation(
            name=f"Starlink_Time_{hour}H",
            ground_station_dataset=GroundStationAtCities.TOP_100,
            shells=shells,
            time_elapsed_seconds=elapsed_seconds,
            verbose=args.verbose,
        )

        current_result = {
            "Time_Hour": hour,
            "Time_Second": elapsed_seconds,
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

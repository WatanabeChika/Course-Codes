"""业务瓶颈实验：对比不同 TM 模型下的性能与链路拥塞基础数据。"""

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
    write_csv_rows,
)

DEFAULT_SUMMARY_CSV_PATH = "./mytests/traffic_matrix_results.csv"

TM_TO_TEST = [
    {
        "name": "High_Population_TM",
        "gs_dataset": GroundStationAtCities.TOP_100,
        "tm_dataset": InternetTrafficAcrossCities.ONLY_POP_100,
    },
    {
        "name": "High_GDP_Population_TM",
        "gs_dataset": GroundStationAtCities.TOP_100,
        "tm_dataset": InternetTrafficAcrossCities.POP_GDP_100,
    },
    {
        "name": "Country_Capitals_TM",
        "gs_dataset": GroundStationAtCities.COUNTRY_CAPITALS,
        "tm_dataset": InternetTrafficAcrossCities.COUNTRY_CAPITALS_ONLY_POP,
    },
]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="对比不同交通矩阵下的网络性能")
    parser.add_argument("--output-csv", default=DEFAULT_SUMMARY_CSV_PATH, help="结果 CSV 路径")
    parser.add_argument("--limit", type=int, default=None, help="只运行前 N 个 TM，用于快速验证")
    parser.add_argument("--export-prefix", default="./", help="详细导出目录前缀")
    parser.add_argument("--verbose", action="store_true", help="打印 LEOCraft 详细日志")
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    start_time = time.perf_counter()

    tm_experiments = TM_TO_TEST[: args.limit] if args.limit else TM_TO_TEST
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

    for exp in tm_experiments:
        exp_name = exp["name"]
        print(f"Evaluating TM={exp_name}")

        leo_con = build_and_route_constellation(
            name=f"Starlink_{exp_name}",
            ground_station_dataset=exp["gs_dataset"],
            shells=shells,
            verbose=args.verbose,
        )

        throughput, stretch = run_throughput_and_stretch(leo_con, exp["tm_dataset"])

        # 导出拥塞分析所需文件
        output_dir = f"{args.export_prefix.rstrip('/')}/Starlink_{exp_name}"
        leo_con.export_routes(output_dir)
        throughput.export_path_selection(output_dir)
        for shell in leo_con.shells:
            shell.export_isls(output_dir)

        current_result = {"Traffic_Matrix": exp_name}
        current_result.update(metrics_from_models(throughput, stretch))
        all_results.append(current_result)

    write_csv_rows(args.output_csv, all_results)
    elapsed_min = round((time.perf_counter() - start_time) / 60, 2)

    print(f"Done. Cases={len(all_results)}")
    print(f"Total time taken: {elapsed_min} minutes")
    print(f"Summary data saved to: {args.output_csv}")


if __name__ == "__main__":
    main()

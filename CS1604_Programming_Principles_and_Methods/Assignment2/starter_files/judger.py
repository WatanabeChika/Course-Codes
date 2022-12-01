import time
import random
import os
import subprocess
import argparse
import sys

exec_name = {
    '1_division': ['main.cpp', 'main'],
    '3_tail_rec': ['main.cpp', 'main'],
    '4_perm_comb': ['main.cpp', 'main'],
    '5_myMath': ['main.cpp', 'main'],
    '6_combinatorics': ['main.cpp', 'main'],
    '7_champagne': ['main.cpp', 'main'],
}
required = {
    '5_myMath': ['myMath.cpp', 'myMath.h'],
    '6_combinatorics': ['combinatorics.cpp', 'combinatorics.h']
}


# exec_name: {'task_name': ['main.cpp', 'executeable']}

def init_args():
    parser = argparse.ArgumentParser('Judger For Homework II')
    parser.add_argument(
        '-T', '--task', choices=list(exec_name.keys()),
        help='the task to judge'
    )
    parser.add_argument(
        '-I', '--input_file', required=True,
        help='the path of input data file'
    )
    parser.add_argument(
        '-O', '--standard_file', required=True,
        help='the path of standard output file'
    )
    parser.add_argument(
        '-S', '--source_dir', default='.',
        help='the folder containing the soruce code'
    )
    parser.add_argument(
        '--timeout', default=1, type=float,
        help='the timeout for judging (in seconds)'
    )

    args = parser.parse_args()
    return args


def get_temp_dir():
    lst = []
    for x in range(10):
        lst.append(chr(ord('A') + random.randint(0, 25)))
    temp_path = ''.join(lst)
    if os.path.exists(temp_path):
        return get_temp_dir()
    else:
        os.mkdir(temp_path)
        return temp_path


def get_random_filename():
    lst = []
    for x in range(10):
        lst.append(chr(ord('A') + random.randint(0, 25)))
    temp_path = ''.join(lst)
    return temp_path


def clr_env(xdir):
    if sys.platform == 'win32':
        subprocess.run(f'del /F /Q \"{xdir}\"', shell=True)
        subprocess.run(f'RD /Q \"{xdir}\"', shell=True)
    else:
        subprocess.run(f'rm -rf \"{xdir}\"', shell=True)


def standard_judger(answer, std, max_score=10):
    # 忽略行末空格和文末空行
    F1 = open(answer)
    F2 = open(std)
    c_answer = F1.read()
    c_std = F2.read()
    c_std = c_std.rstrip().split('\n')
    c_answer = c_answer.rstrip().split('\n')
    F1.close()
    F2.close()
    if len(c_std) != len(c_answer):
        return 'file length differs', 0
    for idx, lin_std in enumerate(c_std):
        lin_ans = c_answer[idx].rstrip()
        lin_std = lin_std.rstrip()
        if lin_std != lin_ans:
            return f'Error Find in Line {idx}', 0
    return 'Correct', max_score


def double_standard_judger(answer, std, max_score=10):
    # 忽略行末空格和文末空行
    F1 = open(answer)
    F2 = open(std)
    c_answer = F1.read()
    c_std = F2.read()
    c_std = c_std.rstrip().split('\n')
    c_answer = c_answer.rstrip().split('\n')
    if len(c_std) != len(c_answer):
        F1.close()
        F2.close()
        return 'file length differs', 0
    for idx, lin_std in enumerate(c_std):
        lin_ans = c_answer[idx].rstrip()
        lin_std = lin_std.rstrip()
        try:
            lin_ans = float(lin_ans)
            lin_std = float(lin_std)
            if abs(lin_std - lin_ans) > 0.01:
                return f'Error > 0.01 in Line {idx}', 0
        except Exception:
            return f'Error in reading float number in Line {idx}', 0
    F1.close()
    F2.close()
    return 'Correct', max_score


def judge(exe, timeout, workdir, args, max_score=10, judger=standard_judger):
    file_name = get_random_filename() + '.out'
    output_file = os.path.join(workdir, file_name)
    exec_file = os.path.join(workdir, exe)
    Fout = open(output_file, 'w')
    Fin = open(args.input_file)

    try:
        subprocess.run(
            [exec_file], check=True, timeout=timeout,
            stdin=Fin, stdout=Fout
        )
    except subprocess.TimeoutExpired:
        Fin.close()
        Fout.close()
        return 'Out of Time Limit!', 0
    except subprocess.CalledProcessError as e:
        Fin.close()
        Fout.close()
        return f'Runtime Error with returncode {e.returncode}', 0

    Fin.close()
    Fout.close()
    return judger(output_file, args.standard_file)


def compile_all(cmds):
    for x in cmds:
        cpr = subprocess.run(x)
        if cpr.returncode != 0:
            return cpr.returncode
        # print(x)
    return 0


if __name__ == '__main__':
    args = init_args()
    workdir = get_temp_dir()

    main_dir = os.path.join(args.source_dir, exec_name[args.task][0])
    exec_dir = os.path.join(workdir, exec_name[args.task][1])

    compiles = {
        '5_myMath': [
            [
                'g++',
                '-c', os.path.join(args.source_dir, 'myMath.cpp'),
                '-o', os.path.join(workdir, 'myMath.o')
            ],
            [
                'g++',
                '-c', os.path.join(args.source_dir, "main.cpp"),
                '-o', os.path.join(workdir, "main.o")
            ],
            [
                'g++',
                os.path.join(workdir, "main.o"),
                os.path.join(workdir, "myMath.o"),
                '-o', os.path.join(workdir, "main")
            ]
        ],
        '6_combinatorics': [
            [
                'g++',
                '-c', os.path.join(args.source_dir, "combinatorics.cpp"),
                '-o', os.path.join(workdir, "combinatorics.o")
            ],
            [
                'g++',
                '-c', os.path.join(args.source_dir, "main.cpp"),
                '-o', os.path.join(workdir, "main.o")
            ],
            [
                'g++',
                os.path.join(workdir, "main.o"),
                os.path.join(workdir, "combinatorics.o"),
                '-o', os.path.join(workdir, "main")
            ]
        ]
    }
    judger_config = {
        '7_champagne': double_standard_judger
    }

    main_dir = os.path.join(args.source_dir, exec_name[args.task][0])
    exec_dir = os.path.join(workdir, exec_name[args.task][1])
    # compile_cmd = f'g++ \"{main_dir}\" -o \"{exec_dir}\" -g -Wall'
    compile_cmd = ['g++', main_dir, '-o', exec_dir, '-g', '-Wall']

    if not os.path.exists(args.input_file):
        print(f'[INFO] Missing Input File')
        clr_env(workdir)
        sys.exit(0)
    if not os.path.exists(args.standard_file):
        print(f'[INFO] Missing Standard Output File')
        clr_env(workdir)
        sys.exit(0)
    if not os.path.exists(main_dir):
        print(f'[INFO] Missing Source Code')
        print('[SCORE] 0')
        clr_env(workdir)
        sys.exit(0)

    if args.task in required:
        for x in required[args.task]:
            ff_dir = os.path.join(args.source_dir, x)
            if not os.path.exists(ff_dir):
                print(f'[INFO] Missing Source Code')
                print('[SCORE] 0')
                clr_env(workdir)
                sys.exit(0)

    if args.task in compiles:
        ret_code = compile_all(compiles[args.task])
    else:
        cp_pro = subprocess.run(compile_cmd)
        ret_code = cp_pro.returncode
    if ret_code != 0:
        print('[INFO] Compile Error\n[SCORE] 0')
    else:
        judger = standard_judger
        if args.task in judger_config.keys():
            judger = judger_config[args.task]

        msg, score = judge(
            exec_name[args.task][1], args.timeout,
            workdir, args, 10, judger
        )
        print(f'[INFO] {msg}\n[SCORE] {score}')
    clr_env(workdir)

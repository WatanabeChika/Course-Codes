import argparse
import difflib
import os
import shutil
import subprocess
import sys
import tempfile

TASKS = [
    ('1_line_wrap', ['main.cpp']),
    ('2_identifierify', ['main.cpp']),
    ('3_fuzzy_search', ['main.cpp']),
    ('4_syllabic_palindrome', ['main.cpp']),
]
TASK_NAMES = [n for n, _ in TASKS]
TASK_NAME_TO_I = dict((n, i) for i, n in enumerate(TASK_NAMES))
IN_SUFFIX = '.in.txt'
OUT_SUFFIX = '.out.txt'
OUT_WRONG_SUFFIX = '.out-wrong.txt'


class Skip(Exception):
    pass


class Wrong(Exception):
    def __init__(self, msg, extra_msg=(), score=0):
        self.msg = msg
        self.extra_msg = extra_msg
        self.score = score

    def __str__(self):
        return self.msg


def parse_args():
    thisdir = os.path.dirname(__file__)
    case_choices = sorted(set(
        f.name[:-len(IN_SUFFIX)]
        for task in TASK_NAMES
        for f in os.scandir(os.path.join(thisdir, 'data', task))
        if f.is_file() and f.name.endswith(IN_SUFFIX)
    ))

    parser = argparse.ArgumentParser()
    parser.add_argument(
        '-T', '--task', type=TASK_NAME_TO_I.__getitem__, action='append', choices=TASK_NAMES,
        help='problem to judge (defaults to all)',
    )
    for i, task in enumerate(TASK_NAMES):
        parser.add_argument(
            f'-{i + 1}', dest='task', action='append_const', const=i,
            help=f'same as --task {task}'
        )
    parser.add_argument(
        '-c', '--case', action='append', choices=case_choices,
        help='sample input/output to test against (defaults to all)'
    )
    parser.add_argument(
        '--timeout', default=1, type=int,
        help='time limit in seconds for your running program'
    )

    args = parser.parse_args()
    if args.task is None:
        args.task = list(range(len(TASKS)))
    else:
        args.task = sorted(set(args.task))
    if args.case is None:
        args.case = case_choices.copy()
    else:
        args.case = sorted(set(args.case))
    return args


def standard_judger_readlines(f):
    result = [l.rstrip() for l in f]
    while result != [] and result[-1] == '':
        result.pop()
    return [l + '\n' for l in result]


DIFFER = difflib.Differ()


def standard_judger(fout, fstd):
    out = standard_judger_readlines(fout)
    std = standard_judger_readlines(fstd)
    diff = list(DIFFER.compare(std, out))
    if any(l[0] != ' ' for l in diff):
        raise Wrong('Incorrect output', extra_msg=(
            '[DIFF] ' + l for l in diff))


def judge(args, task, case, workdir, exe_path):
    task_name = TASK_NAMES[task]
    in_path, std_path = (
        os.path.join(args.thisdir, 'data', task_name, case + suffix)
        for suffix in (IN_SUFFIX, OUT_SUFFIX)
    )

    try:
        fin = open(in_path, mode='rb')
    except FileNotFoundError:
        raise Skip()
    with fin:
        fdout, out_path = tempfile.mkstemp(dir=workdir)
        try:
            subprocess.run(
                [exe_path], check=True, timeout=args.timeout,
                stdin=fin, stdout=fdout
            )
        except subprocess.TimeoutExpired:
            raise Wrong('Timed out')
        except subprocess.CalledProcessError as e:
            raise Wrong(
                f'Runtime error with return code {e.returncode} ({e.returncode:#x})')

    try:
        with open(out_path) as fout:
            with open(std_path) as fstd:
                standard_judger(fout, fstd)
    except Wrong as e:
        if e.score == 0:
            try:
                dst_path = os.path.join(
                    args.thisdir, f'{task_name}.{case}{OUT_WRONG_SUFFIX}')
                shutil.copy2(out_path, dst_path)
                e.msg = f'{e.msg} (output copied to {dst_path})'
            except OSError:
                pass
        raise e


def main():
    args = parse_args()
    args.thisdir = os.path.dirname(__file__)
    for task in args.task:
        tc = None
        workdir = tempfile.mkdtemp()
        try:
            exe_path = compile_and_link(args, task, workdir)
            for case in args.case:
                tc = f'[T{TASK_NAMES[task][0]} c{case}]'
                try:
                    judge(args, task, case, workdir, exe_path)
                except Skip:
                    pass
                except Wrong as e:
                    print(tc, e.msg)
                    sys.stdout.writelines(f'{tc} {l}' for l in e.extra_msg)
                else:
                    print(tc, 'Correct')
        except Wrong as e:
            t = f'[T{TASK_NAMES[task][0]}]'
            print(t, e.msg)
            sys.stdout.writelines(f'{t} {l}' for l in e.extra_msg)
        except OSError as e:
            print(tc if tc else f'[T{TASK_NAMES[task][0]}]', end=' ')
            import traceback
            traceback.print_exception(e, file=sys.stdout)
        finally:
            shutil.rmtree(workdir, ignore_errors=True)


def compile_and_link(args, task, workdir):
    task_name, srcs = TASKS[task]
    src_paths = [os.path.join(args.thisdir, 'source',
                              task_name, src) for src in srcs]
    for src_path in src_paths:
        if not os.path.exists(src_path):
            raise Wrong(f'Missing source file {src_path}')
    obj_paths = [os.path.join(workdir, f'{src}.o') for src in srcs]
    try:
        for src_path, obj_path in zip(src_paths, obj_paths):
            subprocess.run(['g++', '-c', src_path, '-o',
                           obj_path, '-g', '-Wall'], check=True)
    except subprocess.CalledProcessError as e:
        raise Wrong(f'Compile error on {e.args[1][2]}')
    exe_path = os.path.join(workdir, task_name)
    try:
        subprocess.run(['g++'] + obj_paths + ['-o', exe_path], check=True)
    except subprocess.CalledProcessError:
        raise Wrong("Link error (did you forget to define a function, "
                    "or make a typo in the function's name, "
                    "or mistake the function's signature?)")
    return exe_path


if __name__ == '__main__':
    main()

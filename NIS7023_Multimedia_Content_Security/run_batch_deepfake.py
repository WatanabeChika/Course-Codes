import os
import csv
import subprocess

def run_face_swap_from_source(csv_file, source_img_dir, output_dir, run_script_path):
    """
    从原始图片目录批量执行人脸交换命令。

    :param csv_file: task.csv 文件的路径。
    :param source_img_dir: 包含所有原始图片的目录 (e.g., 'imgs')。
    :param output_dir: 保存输出图片的目录 (e.g., 'outputs')。
    :param run_script_path: run.py 脚本的路径。
    """
    # 确保输出目录存在
    os.makedirs(output_dir, exist_ok=True)
    print(f"输出将保存到: {os.path.abspath(output_dir)}")

    # 检查 run.py 是否存在
    if not os.path.exists(run_script_path):
        print(f"错误: 执行脚本 'run.py' 不在路径: {os.path.abspath(run_script_path)}")
        print("请确保 'run.py' 与本脚本在同一目录下。")
        return

    # 读取CSV文件
    try:
        with open(csv_file, mode='r', encoding='gbk') as infile:
            reader = csv.DictReader(infile)
            
            for row in reader:
                if row['task_type'] == 'deepfake':
                    index = row['index']
                    ori_image_name = row['ori_image']
                    target_image_name = row['target_image']

                    source_image_path = os.path.join(source_img_dir, target_image_name)
                    target_image_path = os.path.join(source_img_dir, ori_image_name)
                    output_file_path = os.path.join(output_dir, f"output_{index}.jpg")

                    if not os.path.exists(source_image_path):
                        print(f"警告: [任务 {index}] 跳过，源文件不存在: {source_image_path}")
                        continue
                    if not os.path.exists(target_image_path):
                        print(f"警告: [任务 {index}] 跳过，目标文件不存在: {target_image_path}")
                        continue

                    command = [
                        'python',
                        run_script_path,
                        '-s', source_image_path,
                        '-t', target_image_path,
                        '-o', output_file_path,
                        '--frame-processor', 'face_swapper',
                        '--execution-provider', 'cuda'
                    ]

                    print(f"--- 开始处理任务 {index} ---")
                    print(f"执行: {' '.join(command)}")

                    # 执行命令
                    try:
                        # 使用 subprocess.run 执行命令
                        result = subprocess.run(command, check=True, capture_output=True, text=True, encoding='utf-8', errors='ignore')
                        print(f"成功: [任务 {index}] 已完成, 输出保存在 {output_file_path}")
                        if result.stdout:
                            pass
                            # print("输出:", result.stdout.strip())
                    except FileNotFoundError:
                        print(f"致命错误: 'python' 命令未找到。请确保 Python 已安装并配置在系统 PATH 中。")
                        return # 无法继续，退出函数
                    except subprocess.CalledProcessError as e:
                        print(f"错误: [任务 {index}] 命令执行失败，返回码: {e.returncode}")
                        if e.stdout:
                            print("输出:", e.stdout.strip())
                        if e.stderr:
                            print("错误信息:", e.stderr.strip())
                    except Exception as e:
                        print(f"未知错误: [任务 {index}] 执行时发生错误: {e}")
                    
                    print(f"--- 任务 {index} 处理完成 ---\n")

    except FileNotFoundError:
        print(f"致命错误: CSV 文件未找到于 '{csv_file}'。请确保文件存在。")
    except Exception as e:
        print(f"读取或处理CSV文件时发生错误: {e}")


if __name__ == '__main__':
    # 获取脚本所在目录的绝对路径
    script_dir = os.path.dirname(os.path.abspath(__file__))

    # 定义所有需要的绝对路径
    csv_file_path = os.path.join(script_dir, 'task.csv')
    image_source_directory = os.path.join(script_dir, 'imgs')
    output_directory = os.path.join(script_dir, 'outputs')
    run_py_path = os.path.join(script_dir, '../roop/run.py') # 假设 run.py 在同一目录

    # 执行批量处理
    run_face_swap_from_source(csv_file_path, image_source_directory, output_directory, run_py_path)
    
    print("所有 deepfake 任务已尝试处理。")

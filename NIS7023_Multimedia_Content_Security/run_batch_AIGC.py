import os
import time
import requests


def download_img(img_url, output_path):
    resp = requests.get(img_url)
    resp.raise_for_status()

    with open(output_path, "wb") as f:
        f.write(resp.content)


class ImageGen:
    def __init__(self, api_key):
        self.api_key = api_key
        self.base_url = "https://grsai.dakka.com.cn"

        self.headers = {
            "Content-Type": "application/json",
            "Authorization": f"Bearer {self.api_key}"
        }

        self.max_retry = 3

    def submit_task(self, prompt):
        resp = requests.post(
            f"{self.base_url}/v1/draw/nano-banana",
            headers=self.headers,
            json={
                "model": "nano-banana-pro", # 选择模型，nano-banana-fast
                "prompt": prompt,
                "imageSize": "1K",
                "webHook": "-1"
            },
            timeout=30
        ).json()

        if resp.get("code") != 0:
            raise RuntimeError(f"提交任务失败: {resp}")

        return resp["data"]["id"]


    def generate(self, prompt, output_path):
        task_id = self.submit_task(prompt)
        print("task_id: ", task_id)
        retry = 0

        while True:
            time.sleep(2)

            resp = requests.post(
                f"{self.base_url}/v1/draw/result",
                headers=self.headers,
                json={"id": task_id},
                timeout=30
            ).json()

            if resp.get("code") != 0:
                raise RuntimeError(f"查询失败: {resp}")

            data = resp["data"]
            status = data["status"]

            if status == "succeeded":
                print("生成完成:", data["results"][0]["url"])

                download_img(data["results"][0]["url"], output_path)
                break

            if status == "failed":
                reason = data.get("failure_reason")

                if reason == "error" and retry < self.max_retry:
                    retry += 1
                    print(f"生成失败，重试 {retry}/{self.max_retry}")
                    task_id = self.submit_task(prompt)
                    continue
                else:
                    raise RuntimeError(f"生成失败: {reason}")


def genrerate_images(tasks, api_key=''): # 请替换为你的 API Key
    img_gen = ImageGen(api_key)
    for key in tasks:
        output_path = f'output/{key}.png'
        prompt = tasks[key]

        # 1. [断点续传] 检查文件是否已存在，避免重复消耗 Token
        if os.path.exists(output_path):
            print(f"⏩ [跳过] 任务 {key} 已存在: {output_path}")
            continue

        print(f"🎨 [生成中] 任务 {key}")

        try:
            # 执行生成
            img_gen.generate(prompt, output_path)
            print(f"✅ [成功] 任务 {key} 完成")

            # 4. [防限流] 稍微休息一下，避免请求过快
            time.sleep(1)

        except RuntimeError as e:
            error_msg = str(e)
            # 2. [针对性捕获] 处理内容审查错误
            if "output_moderation" in error_msg:
                print(f"⚠️ [审查拦截] 任务 {key} 触发内容风控，已跳过。")
                reason = "Content Policy (Moderation)"
            else:
                print(f"❌ [运行时错误] 任务 {key}: {error_msg}")
                reason = f"RuntimeError: {error_msg}"

            # 3. [记录日志] 将失败的 prompt 写入文件
            with open("failed_tasks.txt", "a", encoding="utf-8") as f:
                f.write(f"Key: {key} | Time: {time.strftime('%H:%M:%S')} | Reason: {reason}\nPrompt: {prompt}\n{'-'*50}\n")

        except Exception as e:
            # 捕获其他未知错误（如网络断开等）
            print(f"❌ [未知错误] 任务 {key}: {e}")
            with open("failed_tasks.txt", "a", encoding="utf-8") as f:
                f.write(f"Key: {key} | Reason: Unknown Error {e}\nPrompt: {prompt}\n{'-'*50}\n")


def preprocess_prompt(tasks):
    # 添加强化指令，提升生成质量

    ch_prompt = "若未指定风格则默认为写实风格，否则遵循提示词风格设定；严格按照提示词所述画面还原细节，避免无关冗余元素；确保画面自然仿真、细节准确完整。"
    en_prompt = ". Default to photorealistic style unless a style is specified, otherwise follow the prompt's style settings; strictly render the details described in the prompt, avoiding irrelevant or redundant elements; ensure the image is natural and lifelike, with accurate and complete details."

    for key, prompt in tasks.items():
        if '。' in prompt:
            tasks[key] = prompt + ch_prompt
        else:
            tasks[key] = prompt + en_prompt


def png_to_jpg(png_dir, jpg_dir):
    from PIL import Image
    if not os.path.exists(jpg_dir):
        os.makedirs(jpg_dir)
    for filename in os.listdir(png_dir):
        if filename.endswith('.png'):
            png_path = os.path.join(png_dir, filename)
            jpg_path = os.path.join(jpg_dir, filename.replace('.png', '.jpg'))
            with Image.open(png_path) as img:
                rgb_img = img.convert('RGB')
                rgb_img.save(jpg_path, 'JPEG', quality=100)


if __name__ == "__main__":
    import pandas as pd
    df = pd.read_csv("task.csv", encoding="gbk")

    tasks = {}
    for idx, row in df.iterrows():
        if row['task_type'] != 't2i':
            continue
        tasks[row['index']] = row['prompt']

    # task_key = [1]
    # smaller_tasks = {}
    # for k in task_key:
    #     smaller_tasks[k] = tasks[k]
    # preprocess_prompt(smaller_tasks)
    # genrerate_images(smaller_tasks)

    preprocess_prompt(tasks)
    genrerate_images(tasks)
    png_to_jpg(png_dir='output', jpg_dir='output_jpg')
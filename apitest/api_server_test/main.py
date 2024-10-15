import os
import uvicorn

from fastapi import FastAPI
from pydantic import BaseModel
from dotenv import load_dotenv
from groq import Groq

app = FastAPI()

load_dotenv()

client = Groq(
    api_key=os.getenv('GROQ_API_KEY'),
)

class RequestData(BaseModel):
    user_prompt: str
    system_prompt: str
    assistant_prompt: str

def generate_response(user_prompt="", system_prompt="", assistant_prompt=""):
    chat_completion = client.chat.completions.create(
        messages=[
            {
                "role": "user",
                "content": user_prompt,
            },
            {
                "role": "system",
                "content": system_prompt
            },
            {
                "role": "assistant",
                "content": assistant_prompt
            }
        ],
        model="llama3-70b-8192",
    )

    return chat_completion.choices[0].message.content

@app.post("/generate/")
async def generate_response_endpoint(data: RequestData):
    response = generate_response(user_prompt=data.user_prompt, system_prompt=data.system_prompt, assistant_prompt=data.assistant_prompt)
    print("User prompt received: ", data.user_prompt)
    return response

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)

# print(generate_response(user_prompt="hello, how are you?", system_prompt="you are a robot. sometimes you say beep-boop. your answer should be 15 words at most"))

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


class Message(BaseModel):
    role: str
    content: str

class RequestData(BaseModel):
    messages: list[Message]

def generate_response(messages):
    chat_completion = client.chat.completions.create(
        messages=[message.dict() for message in messages],
        model="llama3-70b-8192",
    )

    return chat_completion.choices[0].message.content

@app.post("/generate/")
async def generate_response_endpoint(data: RequestData):
    response = generate_response(data.messages)
    print("User prompt received: ", data.messages[-1].content)
    return response

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)


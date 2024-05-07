const uploadBtn = document.querySelector('#upload-btn');
const fileContent = document.getElementById("file_upload")

function sendFileContent(fileName, fileContentToSend) {
  fetch("https://localhost:8000", {
    method: "POST",
    body: JSON.stringify({
      name: fileName,
      content: fileContentToSend
    }),
    headers: {
      "Content-type": "application/json"
    }
  });
}

fileContent.addEventListener('change', (event) => {
  const file = event.target.files[0];
  const reader = new FileReader();

  reader.readAsText(file);
  reader.onload = (e) => {
    const content = e.target.result;
    console.log(file.name, content);
    sendFileContent(file.name, content);
  }
});

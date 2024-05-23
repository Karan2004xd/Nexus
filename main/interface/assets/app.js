const uploadBtn = document.getElementById('upload-btn');
const fileUpload = document.getElementById('file-upload');
// console.log(fileUpload, uploadBtn);

const fileData = {
  fileName : "",
  fileContent : ""
};

function sendRequest(fileName, fileContent) {
  const url = 'http://localhost:8082';
  fileName = "1wallpaperflare.com_wallpaper.jpg";
  const data = {
    request: 'get',
    filename: fileName,
    // content: fileContent,
    operation: 'get-data'
  };

  fetch(url, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(data)
  })
    .then(response => {
      if (!response.ok) {
        throw new Error('Network response was not ok');
      }
      return response.text();
    })
    .then(data => {
      console.log('Response from server: ', data);
    })
    .catch(error => {
      console.error('Error: ', error);
    });
}

fileUpload.addEventListener('change', (event) => {
  const file = event.target.files[0];
  const reader = new FileReader();

  reader.readAsText(file);
  reader.onload = (e) => {
    const content = e.target.result;
    fileData.fileName = file.name;
    fileData.fileContent = content;
  }
});

uploadBtn.addEventListener('click', (event) => {
  event.preventDefault();
  sendRequest(fileData.fileName, fileData.fileContent);
});

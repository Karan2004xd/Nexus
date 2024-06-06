const btn = document.getElementById("btn");
const uploadBtn = document.getElementById('upload-btn');
const fileUpload = document.getElementById('file-upload');

const fileData = {
  fileName: undefined,
  fileContent: undefined
};

uploadBtn.addEventListener('click', (event) => {
  event.preventDefault();
  const postData = {
    operation: "delete-data",
    filename: fileData.fileName,
    // content: fileData.fileContent
  };
  console.log(postData);

  // console.log(postData);

  fetch('http://localhost:8082/dfs-operations', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(postData),
  })
    .then(response => {
      // window.history.back();
      if (!response.ok) {
        throw new Error('Network response was not ok');
      }
      return response.text();
    })
    .then(data => {
      console.log('Response from server: ', data);
    })
    .catch(error => {
      console.log('Error: ', error);
    });
});

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

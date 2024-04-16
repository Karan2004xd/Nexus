const uploadBtn = document.querySelector('#upload-btn');
console.log(uploadBtn);

uploadBtn.addEventListener('click', (event) => {
  event.preventDefault();
  const file = document.getElementById('file_upload').value;
  console.log(file);
});

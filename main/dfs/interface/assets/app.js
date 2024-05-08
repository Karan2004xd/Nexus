const uploadBtn = document.getElementById('upload-btn');

const url = 'http://localhost:8080';
const data = {
  key1: 'value1',
  key2: 'value2'
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

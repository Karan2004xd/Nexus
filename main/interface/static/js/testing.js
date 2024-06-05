const btn = document.getElementById("btn");

btn.addEventListener('click', () => {
  const postData = {
    operation: "list-data",
    // username: usernameInput.value,
    // password: passwordInput.value 
  };

  // console.log(postData);

  fetch('http://localhost:8082/dfs-operations', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(postData)
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
      console.log('Error: ', error);
    });
});

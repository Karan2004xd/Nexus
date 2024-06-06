const loginForm = document.querySelector(".login-form");
const usernameInput = loginForm.querySelector("#user-field");
const passwordInput = loginForm.querySelector("#pass-field");
const submitBtn = loginForm.querySelector("#submit-btn");

submitBtn.addEventListener('click', (event) => {
  event.preventDefault();
  const postData = {
    // operation: "list-data",
    operation: "check-user",
    username: usernameInput.value,
    password: passwordInput.value 
  };

  // console.log(postData);

  fetch('http://localhost:8082/user', {
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
      window.location.replace("http://localhost:8082/");
    })
    .catch(error => {
      console.log('Error: ', error);
    });
});

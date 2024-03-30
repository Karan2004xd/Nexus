console.log("Hello World");
const btn = document.getElementById('btn');
const inputField = document.querySelector('form');
const submitButton = inputField.querySelector('button');
// console.log(inputField);

btn.addEventListener('click', () => {
  console.log("Don't touch me");
})

submitButton.addEventListener('click', (event) => {
  // event.preventDefault();
  // const inputText = inputField.querySelector("input");
  // console.log(inputText);
});

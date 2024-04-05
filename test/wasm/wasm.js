console.log("Hello World (From JavaScript)");
const getSquareBtn = document.querySelector('button');
let exports;

function run_wasm() {
  WebAssembly.instantiateStreaming(
    fetch("hello.wasm"), {}
  ).then(results => {
      let a = document.querySelector('#a').value;
      let sum = results.instance.exports.findSquare(a);
      let result = document.querySelector('#result');
      console.log(result);
      result.textContent = sum;
    });
};

getSquareBtn.addEventListener('click', run_wasm);

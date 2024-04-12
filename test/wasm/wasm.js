// console.log("Hello World (From JavaScript)");
const getSquareBtn = document.querySelector('button');
const result = document.querySelector('#result');
let exports;

// function run_wasm() {
//   WebAssembly.instantiateStreaming(
//     fetch("hello.wasm"), {}
//   ).then(results => {
//       const a = document.querySelector('#a').value;
//       let sum = results.instance.exports.findSquare2(a);
//       result.textContent = sum;
//     });
// };

// function run_wasm_with_object() {
//   const a = document.querySelector('#a').value;
//   let obj = new Module.TryingWasm(+a);
//   result.textContent = obj.findSquare();
// }

// function testing_derived_classes() {
//   const a = document.querySelector('#a').value;
//   let obj = new Module.DerivedClass();
//   result.textContent = obj.findCube(+a);
// }

function calling_header_file_classes() {
  const a = document.querySelector('#a').value;
  let obj = new Module.TestClass();
  console.log(obj.findSquare(+a))
}

getSquareBtn.addEventListener('click', calling_header_file_classes);

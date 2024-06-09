import { UserOperations } from "../requests/UserOperations.js";

const usernameValue = document.getElementById('username-field');
const passwordValue = document.getElementById('password-field');

const formType = document.getElementById('form-type').value;
const submitBtn = document.getElementById('submit-btn');

const userOperations = new UserOperations();

const SIGN_UP = 'sign-up';
const LOGIN = 'login';
const DEFAULT_ENDPOINT = 'http://localhost:8082';

function redirectToPage(endPoint) {
  const url = DEFAULT_ENDPOINT + endPoint;
  window.location.replace(url);
}

async function getUserOperationsResult() {
  let flag;
  try {
    let result;
    if (formType === SIGN_UP) {
      result = await userOperations.addUser(usernameValue.value, passwordValue.value);
    } else if (formType === LOGIN) {
      result = await userOperations.checkUser(usernameValue.value, passwordValue.value);
    }

    if (result.output === 'true') {
      flag = true;
    }
  } catch (error) {
    console.log(error);
    flag = false;
  }
  return flag;
}

function storeDataInSessionStorage(key, value) {
  sessionStorage.setItem(key, value);
}

submitBtn.addEventListener('click', async (event) => {
  event.preventDefault();
  
  const outputResult = await getUserOperationsResult();
  if (outputResult) {
    if (formType === LOGIN) {
      storeDataInSessionStorage('username', usernameValue.value);
      redirectToPage('/');
    } else if (formType == SIGN_UP) {
      storeDataInSessionStorage('username', usernameValue.value);
      redirectToPage('/login');
    }
  } 
});

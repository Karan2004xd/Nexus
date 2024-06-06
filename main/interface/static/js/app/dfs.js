const sideBarLogo = document.querySelector(".main-sidebar__avatar h1");
const userName = sessionStorage.getItem('username');

sideBarLogo.textContent = userName;

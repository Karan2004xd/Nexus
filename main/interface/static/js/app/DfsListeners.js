import { DfsListenersOperations } from "./DfsListenersOperations.js";

export class DfsListeners {
  #dfsListenersOperations;
  #imageExtensions
  #videoExtensions
  #audioExtensions

  constructor() {
    this.#dfsListenersOperations = new DfsListenersOperations();

    this.#imageExtensions = [
      "jpg",
      "jpeg",
      "png",
      "gif",
      "bmp",
      "tiff",
      "tif",
      "webp",
      "svg",
      "ico",
      "heic",
      "heif",
      "raw",
      "psd"
    ];

    this.#videoExtensions = [
      "mp4",
      "m4v",
      "mkv",
      "mov",
      "avi",
      "wmv",
      "flv",
      "webm",
      "vob",
      "ogv",
      "ogg",
      "3gp",
      "3g2",
      "m2ts",
      "mts",
      "ts",
      "mxf",
      "m4v"
    ];

    this.#audioExtensions = [
      "mp3",
      "wav",
      "aac",
      "flac",
      "alac",
      "wma",
      "ogg",
      "oga",
      "m4a",
      "aiff",
      "opus",
      "amr"
    ];
  }

  #setUserName() {
    const sideBarLogo = document.querySelector(".main-sidebar__avatar h1");
    const userName = sessionStorage.getItem('username');
    sideBarLogo.textContent = userName;
  }

  #putFileListener() {
    const fileUploadBtn = document.querySelector('.file-upload-btn');
    const uploadedFileBtn = document.getElementById('uploaded_file');
    let result;

    fileUploadBtn.addEventListener('click', () => {
      uploadedFileBtn.click();
    });

    uploadedFileBtn.addEventListener('change', (event) => {
      event.preventDefault();
      result = this.#dfsListenersOperations.putDataOperation(event);
    });
    return result;
  }

  #isImage(fileType) {
    return this.#imageExtensions.includes(fileType.toLowerCase());
  }

  #isVideo(fileType) {
    return this.#videoExtensions.includes(fileType.toLowerCase());
  }

  #isAudio(fileType) {
    return this.#audioExtensions.includes(fileType.toLowerCase());
  }

  #removeChildNodes(list, childNodes) {
    const nodeLength = childNodes.length;
    for (let i = 1; i < nodeLength; i++) {
      list.remove(list.children[i]);
    }
  }

  #listDataListenerHelper(filename, fileType) {
    const listElement = document.getElementById('list-item__prototype');
    const newListElement = listElement.cloneNode(true);

    const itemIcon = newListElement.querySelector('#list-item__icon');
    const fileNameField = newListElement.querySelector('.main-files__list-item__filename');
    const fileTypeField = newListElement.querySelector('.main-files__list-item__filetype');

    let fileTypeContent;
    
    if (this.#isImage(fileType)) {
      itemIcon.textContent = "image";
      fileTypeContent = "Image";
    } else if (this.#isVideo(fileType)) {
      itemIcon.textContent = "videocam";
      fileTypeContent = "Video";
    } else if (this.#isAudio(fileType)) {
      itemIcon.textContent = "music_note";
      fileTypeContent = "Audio";
    } else {
      itemIcon.textContent = "draft";
      fileTypeContent = "Document";
    }

    fileNameField.textContent = filename;
    fileTypeField.textContent = fileTypeContent;
    newListElement.id = "";

    return newListElement;
  }

  #setMargin(element, margin, value) {
    let currentMargin = window.getComputedStyle(element);

    if (margin === 'top') {
      currentMargin = currentMargin.marginTop;
    } else if (margin === 'left') {
      currentMargin = currentMargin.marginLeft;
    } else if (margin === 'right') {
      currentMargin = currentMargin.marginRight;
    } else {
      currentMargin = currentMargin.marginBottom;
    }

    currentMargin = parseInt(currentMargin, 10);
    const newMargin = currentMargin + value;
    return `${newMargin}px`;
  }

  async #listDataListener(fileDataMap) {
    const list = document.querySelector('.main-files__list');
    const childNodes = document.querySelectorAll('main-files__list');
    this.#removeChildNodes(list, childNodes);

    for (const [filename, fileType] of fileDataMap) {
      const listItem = this.#listDataListenerHelper(filename, fileType);
      list.appendChild(listItem);

      const newMargin = this.#setMargin(list, 'top', 10);
      list.style.marginTop = newMargin;
    }
  }

  #getFilteredData(conditionMethod, fileDataMap) {
    const filteredDataMap = new Map();
    for (const [filename, fileType] of fileDataMap) {
      if (conditionMethod(fileType)) {
        filteredDataMap.set(filename, fileType);
      }
    }
    return filteredDataMap;
  }

  #filterData(fileType, fileDataMap) {
    let resultDataMap;
    switch (fileType) {
      case 'image':
        resultDataMap = this.#getFilteredData(this.#isImage, fileDataMap);
        break;
      case 'video':
        resultDataMap = this.#getFilteredData(this.#isVideo, fileDataMap);
        break;
      case 'audio':
        resultDataMap = this.#getFilteredData(this.#isAudio, fileDataMap);
      default:
        break;
    }
    return resultDataMap;
  }

  async #getMainDataMap() {
    const mainData = sessionStorage.getItem('mainData');
    let mainDataMap;
    if (mainData === 'home') {
      mainDataMap = await this.#dfsListenersOperations.getFileData();
    } else if (mainData === 'trash') {
      mainDataMap = await this.#dfsListenersOperations.getTrashFileData();
    }
    return mainDataMap;
  }

  async #listData() {
    let type = sessionStorage.getItem('listData');
    if (type === undefined) {
      type = 'home';
      this.#setListDataCacheVariable('home');
    }
    let fileDataMap;
    switch (type) {
      case 'home':
        fileDataMap = await this.#dfsListenersOperations.getFileData();
        break;
      
      case 'trash':
        fileDataMap = await this.#dfsListenersOperations.getTrashFileData();
        sessionStorage.setItem('mainData', 'trash');
        break;

      case 'image':
        fileDataMap = this.#filterData('image', await this.#getMainDataMap());
      default:
        break;
    }
    return fileDataMap;
  }

  #setListDataCacheVariable(varValue) {
    sessionStorage.setItem('listData', varValue);
  }

  #listTrashDataListener() {
    const deletedItemsBtn = document.getElementById('deleted-items__btn');

    deletedItemsBtn.addEventListener('click', () => {
      window.location.reload();
      this.#setListDataCacheVariable('trash');
      sessionStorage.setItem('mainData', 'trash');
    });
  }

  #listHomeItemsListener() {
    const homeBtn = document.getElementById('home-btn');

    homeBtn.addEventListener('click', () => {
      window.location.reload();
      this.#setListDataCacheVariable('home');
      sessionStorage.setItem('mainData', 'home');
    });
  }

  async #setFilterButtonsListeners() {
    const filterButtons = querySelectorAll('.main-dfs__categories__btn');
    const fileDataMap = await this.#listData();

    for (const buttons of filterButtons) {
      switch (buttons.id) {
        case 'image-category':
          break;
        case 'document-category':
          break;
        case 'audio-category':
          break;
        case 'video-category':
          break;
        default:
          break;
      }
    }
  }

  async setEventListeners() {
    this.#setUserName();
    this.#putFileListener();
    this.#listDataListener(await this.#listData());
    this.#listHomeItemsListener();
    this.#listTrashDataListener();
  }
}

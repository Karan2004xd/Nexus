import { DfsListenersOperations } from "./DfsListenersOperations.js";

export class DfsListeners {
  #dfsListenersOperations;

  constructor() {
    this.#dfsListenersOperations = new DfsListenersOperations();
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
    const imageExtensions = ["jpg", "jpeg", "png", "gif", "bmp",
      "tiff", "tif", "webp", "svg", "ico", "heic", "heif","raw",
      "psd"
    ];
    return imageExtensions.includes(fileType.toLowerCase());
  }

  #isVideo(fileType) {
    const videoExtensions = ["mp4", "m4v", "mkv", "mov", "avi", "wmv",
      "flv", "webm", "vob", "ogv", "ogg", "3gp", "3g2", "m2ts",
      "mts", "ts", "mxf","m4v"
    ];
    return videoExtensions.includes(fileType.toLowerCase());
  }

  #isAudio(fileType) {
    const audioExtensions = ["mp3", "wav", "aac", "flac", "alac",
      "wma", "ogg", "oga", "m4a", "aiff", "opus", "amr"
    ];
    return audioExtensions.includes(fileType.toLowerCase());
  }

  #isDocument(fileType) {
    const imageExtensions = ["jpg", "jpeg", "png", "gif", "bmp",
      "tiff", "tif", "webp", "svg", "ico", "heic", "heif","raw",
      "psd"
    ];

    const videoExtensions = ["mp4", "m4v", "mkv", "mov", "avi", "wmv",
      "flv", "webm", "vob", "ogv", "ogg", "3gp", "3g2", "m2ts",
      "mts", "ts", "mxf","m4v"
    ];

    const audioExtensions = ["mp3", "wav", "aac", "flac", "alac",
      "wma", "ogg", "oga", "m4a", "aiff", "opus", "amr"
    ];

    return !imageExtensions.includes(fileType.toLowerCase()) &&
           !videoExtensions.includes(fileType.toLowerCase()) &&
           !audioExtensions.includes(fileType.toLowerCase());
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

      const newMargin = this.#setMargin(list, 'top', 30);
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
        break;
      case 'document':
        resultDataMap = this.#getFilteredData(this.#isDocument, fileDataMap);
        break;
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
    if (!type) {
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
        break;

      case 'video':
        fileDataMap = this.#filterData('video', await this.#getMainDataMap());
        break;

      case 'audio':
        fileDataMap = this.#filterData('audio', await this.#getMainDataMap());
        break;

      case 'document':
        fileDataMap = this.#filterData('document', await this.#getMainDataMap());
        break;

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

  #setFilterButtonsListeners() {
    const filterButtons = document.querySelectorAll('.main-dfs__categories__btn');

    for (const buttons of filterButtons) {
      buttons.addEventListener('click', () => {
        switch (buttons.id) {
          case 'image-category':
            this.#setListDataCacheVariable('image');
            break;
          case 'document-category':
            this.#setListDataCacheVariable('document');
            break;
          case 'audio-category':
            this.#setListDataCacheVariable('audio');
            break;
          case 'video-category':
            this.#setListDataCacheVariable('video');
            break;
          default:
            break;
        }
        window.location.reload();
      });
    }
  }

  #loadDashboardUi() {
    if (!sessionStorage.getItem('mainData')) {
      const homeBtn = document.getElementById('home-btn');
      homeBtn.click();
    }
  }

  #deleteDataListener() {
    const listElement = document.querySelectorAll('.main-files__list-item');
    for (const element of listElement) {
      const filename = element.querySelector('.main-files__list-item__filename').textContent;
      const deleteFileBtn = element.querySelector('.main-files__list-item__delete-btn');

      deleteFileBtn.addEventListener('click', async () => {
        const currentData = sessionStorage.getItem('mainData');
        if (currentData === 'home') {
          await this.#dfsListenersOperations.deleteFileData(filename);
        } else if (currentData === 'trash') {
          await this.#dfsListenersOperations.deleteTrashFileData(filename);
        }
        window.location.reload();
      });
    }
  }

  async #getFileTypeFromUrl(filename) {
    const currentData = sessionStorage.getItem('mainData');

    let fileDataMap;
    if (currentData === 'home') {
      fileDataMap = await this.#dfsListenersOperations.getFileData(filename);
    } else if (currentData === 'trash') {
      fileDataMap = await this.#dfsListenersOperations.getTrashFileData(filename);
    }

    const fileType = fileDataMap.get(filename);
    const downloadFilename = `${filename}`;

    let mainFileType;
    if (this.#isImage(filename)) {
      mainFileType = 'image';
    } else if (this.#isVideo(filename)) {
      mainFileType = 'video';
    } else if (this.#isAudio(filename)) {
      mainFileType = 'audio';
    } else {
      mainFileType = 'text';
    }

    const mimeType = `${mainFileType}/${fileType}`;
    return {downloadFilename : downloadFilename, mimeType: mimeType};
  }

  #downloadDataListener() {
    const listElement = document.querySelectorAll('.main-files__list-item');
    for (const element of listElement) {
      const filename = element.querySelector('.main-files__list-item__filename').textContent;
      const downloadFileBtn = element.querySelector('.main-files__list-item__download-btn');

      downloadFileBtn.addEventListener('click', async () => {
        let fileData;
        const currentData = sessionStorage.getItem('mainData');
        if (currentData === 'home') {
          fileData = await this.#dfsListenersOperations.getDataOperation(filename);
        } else if (currentData === 'trash') {
          fileData = await this.#dfsListenersOperations.getTrashDataOperation(filename);
        }

        const downloadFileDetails = await this.#getFileTypeFromUrl(filename);

        let byteCharacters = atob(fileData.output);
        let byteNumbers = new Array(byteCharacters.length);

        for (let i = 0; i < byteCharacters.length; i++) {
          byteNumbers[i] = byteCharacters.charCodeAt(i);
        }

        let byteArray = new Uint8Array(byteNumbers);
        let blob = new Blob([byteArray], { type: 'application/octet-stream' });
        
        let blobUrl = URL.createObjectURL(blob);

        const a = document.createElement('a');
        a.href = blobUrl;
        a.download = downloadFileDetails.downloadFilename; 
        document.body.appendChild(a);
        a.click();

        setTimeout(() => {
          document.body.removeChild(a);
          URL.revokeObjectURL(blobUrl);
        }, 0);
      });
    }
  }

  async setEventListeners() {
    this.#setUserName();
    this.#putFileListener();
    this.#listDataListener(await this.#listData());
    this.#setFilterButtonsListeners();
    this.#listHomeItemsListener();
    this.#listTrashDataListener();
    this.#deleteDataListener();
    this.#downloadDataListener();
    this.#loadDashboardUi();
  }
}

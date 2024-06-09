import { DfsOperations } from "../requests/DfsOperations.js"

export class DfsListenersOperations {
  #dfsRequestOperations
  #fileData
  #trashFileData

  constructor() {
    this.#dfsRequestOperations = new DfsOperations();
    this.#fileData = new Map();
  }

  async getFileData() {
    await this.#listDataOperation();
    return this.#fileData;
  }

  async getTrashFileData() {
    await this.#listTrashDataOperation();
    return this.#trashFileData;
  }

  #setTrashFileData(fileData) {
    this.#trashFileData = new Map();
    const len = fileData.name.length;

    for (let i = 0; i < len; i++) {
      this.#trashFileData.set(fileData.name[i], fileData.type[i]);
    }
  }

  #setFileData(fileData) {
    this.#fileData = new Map();
    const len = fileData.name.length;

    for (let i = 0; i < len; i++) {
      this.#fileData.set(fileData.name[i], fileData.type[i]);
    }
  }

  #arrayBufferToBase64(buffer) {
    let binary = '';
    const bytes = new Uint8Array(buffer);
    const len = bytes.byteLength;
    for (let i = 0; i < len; i++) {
      binary += String.fromCharCode(bytes[i]);
    }
    return btoa(binary);
  }

  async putDataOperation(listenerEvent) {
    const file = listenerEvent.target.files[0];
    const reader = new FileReader();

    reader.readAsArrayBuffer(file);
    reader.onload = async (event) => {
      const arrayBuffer = event.target.result;
      const base64String = this.#arrayBufferToBase64(arrayBuffer);

      const fileData = {
        filename: file.name,
        content: base64String 
      };

      try {
        const result = await this.#dfsRequestOperations.putData(fileData.filename, fileData.content);
        window.location.reload();
        return result;
      } catch (error) {
        throw Error("false");
      }
    };
  }

  async #listDataOperation() {
    try {
      const result = await this.#dfsRequestOperations.listData();
      this.#setFileData(result);
    } catch (error) {}
  }

  async #listTrashDataOperation() {
    try {
      const result = await this.#dfsRequestOperations.listTrashData();
      this.#setTrashFileData(result);
    } catch (error) {}
  }

  async getDataOperation(filename) {
    try {
      const result = await this.#dfsRequestOperations.getData(filename);
      return result;
    } catch (error) {
      console.log(error);
    }
  }

  async getTrashDataOperation(filename) {
    try {
      const result = await this.#dfsRequestOperations.getTrashData(filename);
      return result;
    } catch (error) {}
  }

  async #deleteDataOperation(filename) {
    try {
      return await this.#dfsRequestOperations.deleteData(filename);
    } catch (error) {}
  }

  async #deleteTrashDataOperation(filename) {
    try {
      return await this.#dfsRequestOperations.deleteTrashData(filename);
    } catch (error) {}
  }

  async deleteFileData(filename) {
    await this.#deleteDataOperation(filename);
    this.#fileData.delete(filename);
  }

  async deleteTrashFileData(filename) {
    await this.#deleteTrashDataOperation(filename);
    this.#trashFileData.delete(filename);
  }
}

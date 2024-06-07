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

  putDataOperation(listenerEvent) {
    const file = listenerEvent.target.files[0];
    const reader = new FileReader();

    reader.readAsText(file);
    reader.onload = async (event) => {
      const content = event.target.result;
      const fileData = {
        filename: file.name,
        content: content
      };
      try {
        const result = await this.#dfsRequestOperations.putData(fileData.filename, fileData.content);
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
    let result = this.#fileData.get(filename);
    if (result !== undefined) {
      return result;
    }
    try {
      result = await this.#dfsRequestOperations.getData(filename);
      return result;
    } catch (error) {}
  }

  async getTrashDataOperation(filename) {
    let result = this.#trashFileData.get(filename);
    if (result !== undefined) {
      return result;
    }
    try {
      result = await this.#dfsRequestOperations.getData(filename);
      return result;
    } catch (error) {}
  }

  async deleteDataOperation(filename) {
    try {
      return await this.#dfsRequestOperations.deleteData(filename);
    } catch (error) {}
  }

  async deleteTrashDataOperation(filename) {
    try {
      return await this.#dfsRequestOperations.deleteTrashData(filename);
    } catch (error) {}
  }
}

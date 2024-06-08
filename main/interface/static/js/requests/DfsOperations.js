import { OperationHandler } from "./OperationHandler.js";

export class DfsOperations {
  #operationHandler;
  #defaultEndPoint;

  constructor() {
    this.#operationHandler = new OperationHandler();
    this.#defaultEndPoint = "/dfs";
  }

  async listData() {
    const requestData = {
      operation: 'list-data'
    };
    return await this.#operationHandler.getResponse(this.#defaultEndPoint, requestData);
  }

  async listTrashData() {
    const requestData = {
      operation: 'list-trash-data'
    };
    return await this.#operationHandler.getResponse(this.#defaultEndPoint, requestData);
  }

  async putData(filename, content) {
    const requestData = {
      operation: 'put-data',
      filename: filename,
      content: content
    };
    return await this.#operationHandler.getResponse(this.#defaultEndPoint, requestData);
  }

  async getData(filename) {
    const requestData = {
      operation: 'get-data',
      filename: filename,
    };
    return await this.#operationHandler.getResponse(this.#defaultEndPoint, requestData);
  }

  async getTrashData(filename) {
    const requestData = {
      operation: 'get-trash-data',
      filename: filename,
    };
    return await this.#operationHandler.getResponse(this.#defaultEndPoint, requestData);
  }

  async deleteData(filename) {
    const requestData = {
      operation: 'delete-data',
      filename: filename,
    };
    return await this.#operationHandler.getResponse(this.#defaultEndPoint, requestData);
  }

  async deleteTrashData(filename) {
    const requestData = {
      operation: 'delete-trash-data',
      filename: filename,
    };
    return await this.#operationHandler.getResponse(this.#defaultEndPoint, requestData);
  }
}

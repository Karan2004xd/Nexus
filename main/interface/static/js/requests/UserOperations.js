import { OperationHandler } from "./OperationHandler.js";

export class UserOperations {
  #operationsHandler;
  #defaultEndpoint;
  #ADD_USER;
  #CHECK_USER;

  constructor() {
    this.#operationsHandler = new OperationHandler();
    this.#defaultEndpoint = '/user';
    this.#ADD_USER = 'add-user';
    this.#CHECK_USER = 'check-user';
  }

  async addUser(username, password) {
    const requestData = {
      operation: this.#ADD_USER,
      username: username,
      password: password
    }
    
    return await this.#operationsHandler.getResponse(this.#defaultEndpoint, requestData);
  }

  async checkUser(username, password) {
    const requestData = {
      operation: this.#CHECK_USER,
      username: username,
      password: password
    }
    
    return await this.#operationsHandler.getResponse(this.#defaultEndpoint, requestData);
  }
}

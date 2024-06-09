export class OperationHandler {
  #mainEndpoint;

  constructor() {
    this.#mainEndpoint = 'http://localhost:8082';
  }

  async getResponse(requestEndPoint, requestData) {
    const requestUrl = this.#mainEndpoint + requestEndPoint;

    try {
      const response = await fetch(requestUrl, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json'
        },
        body: JSON.stringify(requestData)
      });

      if (response.ok) {
        const data = await response.json();
        return data;
      }
    } catch (error) {
      console.log('Error: ', error);
      throw error;
    }
  }
}

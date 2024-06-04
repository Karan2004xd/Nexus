document.getElementById('sendRequest').addEventListener('click', async () => {
  const requestPayload = JSON.parse(document.getElementById('requestInput').value);
  const requestId = new Date().getTime().toString();

  try {
    const response = await fetch('http://localhost:3000/dfs', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        requestId: requestId,
        payload: requestPayload,
      }),
    });
    const responseData = await response.text();
    console.log('Response from server:', responseData);

    document.getElementById('response').textContent = response.data;
  } catch (error) {
    console.error('Error sending request:', error);
    alert('Failed to send request');
  }
});

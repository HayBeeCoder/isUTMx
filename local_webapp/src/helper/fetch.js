
const fetchData = async (url, options = {}, retries = 3, timeout = 5000) => {
    const controller = new AbortController(); // Allows us to abort the request if it takes too long
    const signal = controller.signal;

    // Set a timeout for the fetch request
    const fetchTimeout = setTimeout(() => controller.abort(), timeout);

    try {
        const response = await fetch(url, { ...options, signal });

        // Clear the timeout if the request is successful
        clearTimeout(fetchTimeout);

        // Check if the response status is OK (status in the range 200-299)
        if (!response.ok) {
            const errorMessage = `Fetch error: ${response.status} ${response.statusText}`;
            throw new Error(errorMessage);
        }

        // Parse and return JSON data
        return await response.json();

    } catch (error) {
        // Handle request timeout or fetch errors
        if (error.name === 'AbortError') {
            throw new Error('Request timed out');
        }

        if (retries > 0) {
            console.warn(`Retrying fetch... attempts left: ${retries - 1}`);
            return fetchData(url, options, retries - 1, timeout); // Retry request
        } else {
            throw new Error(`Fetch failed: ${error.message}`);
        }
    }
};


export default fetchData;
function capitalize(word) {
    if (!word) return ''; // Check if word is not empty or undefined
    return word.charAt(0).toUpperCase() + word.slice(1).toLowerCase();
  }
  
  export default capitalize;
// src/firebase.js
import { initializeApp } from 'firebase/app';
import { getFirestore } from 'firebase/firestore'; // For Firestore database
import { getAuth } from 'firebase/auth'; // For Firebase authentication
import { getStorage } from 'firebase/storage'; // For Firebase storage

const firebaseConfig = {
    apiKey: "AIzaSyDXN-3XKZAJ2emN96ziFyqdNJI39iWoXG0",
    authDomain: "isutmxx.firebaseapp.com",
    databaseURL: "https://isutmxx-default-rtdb.firebaseio.com",
    projectId: "isutmxx",
    storageBucket: "isutmxx.appspot.com",
    messagingSenderId: "813438085086",
    appId: "1:813438085086:web:b34f9fd6d0ac638e815f4f"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);

// Initialize services
const db = getFirestore(app);
const auth = getAuth(app);
const storage = getStorage(app);

export { db, auth, storage };

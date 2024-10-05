import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'
// import manifestPlugin from ';

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [react()],


  build: {
    // manifest: true,
    manifest: 'assets/asset-manifest.json', // The manifest file will be generated inside the assets folder
  
     },
  server: {
    proxy: {
      "/api": "http://isutmx.local",
    },
  },
})

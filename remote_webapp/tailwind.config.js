
/** @type {import('tailwindcss').Config} */
export default {

  content: [
    "./index.html",
    "./src/**/*.{js,ts,jsx,tsx}",
  ],
  theme: {
    fontFamily: {

      'digital': ['Digital'],
      'body': ['"Open Sans"'],
      "faster": ['"Faster One"']
    },
    extend: {

      colors: {
        'primary': '#6c43e8',
      },

    },
  },
  plugins: [require("tailwind-scrollbar"), require("tailwind-scrollbar-hide")],

}
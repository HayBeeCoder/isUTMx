/** @type {import('tailwindcss').Config} */
export default {
  content: [
    "./index.html",
    "./src/**/*.{js,ts,jsx,tsx}",
  ],
  theme: {
    fontFamily: {

      'digital': ['Digital'],
      'body': ['OpenSans'],
      "faster": ['"Faster One"']
    },
    extend: {
      dropShadow: {
        'started': [
          '4px 4px 8px rgba(74,222,128 , 0.17)', '2px 2px 6px rgba(74,222,128, 0.26)'
        ],
        'stopped': [
          '4px 4px 8px rgba(224, 49, 49, 0.25)',
          '2px 2px 6px rgba(224, 49, 49, 0.15)'
        ],
        'not-started': [
          '4px 4px 8px rgba(255, 221, 0, 0.25)',  // Soft yellow outer shadow
          '2px 2px 6px rgba(255, 221, 0, 0.15)'   // Softer yellow inner shadow
        ]

      },
      keyframes: {
        blink: {
          '0%, 100%': { opacity: '1' },
          '50%': { opacity: '0' },
        },
      },
      animation: {
        blink: 'blink 1s infinite',
      },
      colors: {
        'primary': '#6c43e8',
      },

    },
  },
  plugins: [require("tailwind-scrollbar"), require("tailwind-scrollbar-hide")],

}


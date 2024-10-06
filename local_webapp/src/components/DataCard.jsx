// src/components/DataCard.jsx
import React from 'react';

const DataCard = ({ title, value, valueSize = "big", className, theme }) => {

  const obj = {
    "small": "text-sm",
    "big": "text-xl md:text-2xl",
    "bigger": "text-4xl md:text-6xl"
  }

  const cardTheme = {
    "blueBg": "bg-[#6c43e8] text-white",
    "default": "text-[#6C43E8]"
  }

  return (
    <div className={`p-4 border-[2px] border-[#F4F0FF]  rounded-md ${className} ${cardTheme["blueBg"]}`}>
      <p className="  text-[12px] text-xs md:text-sm font-extrabold">{title}</p>
      <p className={
        `font-digital   text-right break-words ${obj[valueSize]} `

      }>{value}</p>
    </div>
  );
};

export default DataCard;

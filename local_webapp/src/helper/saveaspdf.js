import jsPDF from 'jspdf';
import html2canvas from 'html2canvas';
import autoTable from 'jspdf-autotable';
import { BASE64ENCODEDFORMAT } from '../constants';
import capitalize from './capitalize';

const tests = {
    tension: "Extension",
    compression: "Compression "
}

const saveAsPDF = (tableData, test_type, sensor_rating, target_force, target_extension, pdf_name) => {
    const chart = document.getElementById("graph");

    html2canvas(chart, { scale: 1 }).then((canvas) => {
        const imgData = canvas.toDataURL('image/png');

        // Create a new jsPDF instance
        const pdf = new jsPDF('portrait', 'pt', 'a4');
        let title = 'ISUTMX'
        // Add "ISUTMX" to top-left corner
        pdf.addFileToVFS('faster.ttf', BASE64ENCODEDFORMAT); // Add your base64 font here
        pdf.addFont('faster.ttf', 'faster', 'normal');
        pdf.setFont('faster', 'normal');
        let pageWidth = pdf.internal.pageSize.getWidth();
        let titleX = (pageWidth - pdf.getTextWidth(title)) / 2; // Center the title
        pdf.setFontSize(20);
        pdf.text(title, titleX, 30); // Centered title at Y position 30

        pdf.setFont("montserrat")



        pdf.setFontSize(12);

        pdf.text(`Test type: ${capitalize(test_type)}`, 60, 60);
        pdf.text(`Sensor rating: ${sensor_rating / 9.81}kg = ${sensor_rating}N`, 60, 80);
        pdf.text(`Target force: ${target_force ? target_force : "-----"}N`, 60, 100);
        pdf.text(`Target extension: ${target_extension ? target_extension : "-----"}mm`, 60, 120);

        pdf.setFont("montserrat", "bold");

        title = "Graph"
        titleX = (pageWidth - pdf.getTextWidth(title)) / 2;
        pdf.text(title, titleX, 180);


        pdf.addImage(imgData, 'PNG', 20, 190, 560, 300);


        title = "Table"
        titleX = (pageWidth - pdf.getTextWidth(title)) / 2;
        pdf.text(title, 300, 540);


        autoTable(pdf, {
            head: [['Force', tests[test_type]]],
            body: tableData.map(row => [row.force_in_newton, row.displacement_in_mm]),
            startY: 550,
            tableWidth: 500,
            margin: {
                left: 60
            },
            columnStyles: {
                0: { cellWidth: 250 }, // Set the width of the first column
                1: { cellWidth: 250 }, // Set the width of the second colum
            },
            styles: { halign: 'center' },
            theme: 'grid',
            headStyles: {
                fillColor: [108, 67, 232],
                textColor: 255,
            },

        });

        // Save the PDF
        pdf.save(`${pdf_name}.pdf`);
    })

};

export default saveAsPDF;